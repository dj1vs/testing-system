#include "myclient.h"

MyClient::MyClient(const QString& strHost, int nPort, QWidget *parent)
    : QMainWindow(parent)
{
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort);

    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );
    d = new QErrorMessage(this);

    this->setMinimumSize(WINW, WINH);

    setAuthorizationWindow();
}

MyClient::~MyClient()
{
}

void MyClient::setAuthorizationWindow()
{
    authorizeLoginLabel = new QLabel(this);
    authorizeLoginLabel->setText("Login:");
    authorizeLoginLabel->setFont(QFont("Arial", 15));
    authorizeLoginLabel->resize(100, 25);

    authorizePasswordLabel = new QLabel(this);
    authorizePasswordLabel->setText("Password:");
    authorizePasswordLabel->setFont(QFont("Arial", 15));
    authorizePasswordLabel->resize(100, 20);

    authorizeLogin = new QLineEdit(this);
    authorizeLogin->resize(200, 20);
    authorizePassword = new QLineEdit(this);
    authorizePassword->resize(200, 20);

    authorize = new QPushButton("&Log in", this);
    authorize->setFont(QFont("Arial", 15));

    QFormLayout *fl = new QFormLayout();
    fl->addRow(authorizeLoginLabel, authorizeLogin);
    fl->addRow(authorizePasswordLabel, authorizePassword);

    authorizeLayout = new QVBoxLayout();
    authorizeLayout->addLayout(fl);
    authorizeLayout->addWidget(authorize);

    QWidget *w = new QWidget();
    w->setLayout(authorizeLayout);
    setCentralWidget(w);

    connect(authorize, &QPushButton::clicked, this, [this] () {logInToSystem();});
}

void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < static_cast<quint16>(sizeof(quint16))) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;
        solveMsg(str);
        m_nNextBlockSize = 0;
    }
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    showError(strError);
}

void MyClient::slotSendToServer(QString msg)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out << quint16(0) << QTime::currentTime() << QString(msg);

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
}

void MyClient::slotConnected()
{
    showMsg("Received the connected() signal");
}

void MyClient::solveMsg(QString msg)
{
    if(msg[0] != '{')
    {
        qDebug() << "Recieved invalid package:";
        qDebug() << msg;
        return;
    }
    QString cmd = cutArg(msg, "cmd");
    if(cmd == "authorize")
    {
        if(cutArg(msg, "status") == "0")
        {
            QString role = cutArg(msg, "role");
            showMsg("Succesfuly logged in!\nRole: " + role);
            hideAuthorizationWindow();
            if(role == "admin+")
                setAdminPlusWindow();
            else if(role == "admin")
                setAdminWindow();
        }
        else
           showError("Invalid login/password :(");
    }
    else if(cmd == "add group")
    {
        int r = cutArg(msg, "status").toInt();
        switch(r)
        {
            case 0:
                showMsg("Group added succesfully");
                addGroupTitle->setText("");
            break;
            case 1:
                showError("No such  teacher");
            break;
            case 2:
                showError("Введённые имя и фамилия не принадлежат учителю");
            break;
            }
    }
    else if(cmd == "add to group")
    {
        int r = cutArg(msg, "status").toInt();
        switch(r)
        {
        case 0:
            showMsg("added to group succesfully");
            addToGroupName->setText("");
            addToGroupSurname->setText("");
            break;
        case 1:
            showError("Cant find group with entered name");
            break;
        case 2:
            showError("Cant find student with entered name");
            break;
        case 3:
            showError("Already added");
            break;
        }
    }
    else if(cmd == "appoint")
    {
        int r = cutArg(msg, "status").toInt();
        switch(r)
        {
        case 0:
            showMsg("successfully appointed");
            appointGroupTitle->setText("");
            break;
        case 1:
            showError("Введённые имя и фамилия не принадлежат ни одному учителю");
            break;
        case 2:
            showError("No groups with this name");
            break;
        }
    }
    else if (cmd == "add user")
    {
        int r = cutArg(msg, "status").toInt();
        switch(r)
        {
        case 0:
            showMsg("User succesfully added");
            break;
        case 1:
            showError("This user already exists");
            break;
        }
    }
    else if(cmd == "view all results")
    {
        if(cutArg(msg, "status") == "sended")
        {
            emit allResultsCollected();
            return;
        }
        else if(cutArg(msg, "status") == "started")
        {
            allResultsList.clear();
            return;
        }
        QList <QString> buf = {cutArg(msg, "testname"), cutArg(msg, "date"),
                    cutArg(msg, "subject"),cutArg(msg, "studentsname"),
                    cutArg(msg, "studentssurname"),cutArg(msg, "percent"),};
        allResultsList.push_back(buf);
    }
    else if(cmd == "view all groups")
    {
        if(cutArg(msg, "status") == "sended")
        {
            emit allGroupsCollected();
            return;
        }
        else if(cutArg(msg, "status") == "started")
        {
            allGroupsList.clear();
            return;
        }
        allGroupsList.push_back(cutArg(msg, "name"));
    }
    else if (cmd == "view group teachers")
    {
        QString status = cutArg(msg, "status");
        if(status == "sended")
            emit groupTeachersCollected();
        else if(status == "started")
            groupTeachers.clear();
        else
            groupTeachers.push_back({cutArg(msg, "name"), cutArg(msg, "surname")});
    }
    else if (cmd == "view group students")
    {
        QString status = cutArg(msg, "status");
        if(status == "sended")
            emit groupStudentsCollected();
        else if(status == "started")
            groupStudents.clear();
        else
            groupStudents.push_back({cutArg(msg, "name"), cutArg(msg, "surname")});
    }
}

void MyClient::logInToSystem()
{
    QString login = authorizeLogin->text();
    QString password = authorizePassword->text();
    QString str = "{cmd='authorize';login='" + login;
    str += "';pass='" + password + "';}";
    slotSendToServer(str);
    authorizeLogin->setText("");
    authorizePassword->setText("");
}

QString MyClient::cutArg(QString str, QString cmd)
{
    int pos1 = str.indexOf(cmd) + cmd.size() + 2;
    int pos2 = str.midRef(pos1, str.size()).indexOf("'");
    return str.mid(pos1, pos2);
}

void MyClient::showError(QString err)
{
    d->showMessage(err);
}

void MyClient::showMsg(QString msg)
{
    this->statusBar()->showMessage(msg);
}

void MyClient::hideAuthorizationWindow()
{
    authorizeLoginLabel->hide();
    authorizePasswordLabel->hide();
    authorizeLogin->hide();
    authorizePassword->hide();
    authorize->hide();
}

void MyClient::setAdminPlusWindow()
{
    addStudentsGroup = new QPushButton("&Add group", this);
    addStudentsToGroup = new QPushButton("&Add to group", this);
    appointGroup = new QPushButton("&Appoint group", this);
    addUser = new QPushButton("&Add user", this);

    addStudentsGroup->show();
    addStudentsToGroup->show();
    appointGroup->show();
    addUser->show();

    connect(addStudentsGroup, &QPushButton::clicked, this,
            [this] () {hideAdminPlusWindow(); setAddGroupWindow();});
    connect(addStudentsToGroup, &QPushButton::clicked, this,
            [this] () {hideAdminPlusWindow(); setAddToGroupWindow();});

    connect(appointGroup, &QPushButton::clicked, this,
            [this] () {hideAdminPlusWindow(); setAppointGroupWindow();});
    connect(addUser, &QPushButton::clicked, this,
            [this] () {hideAdminPlusWindow(); setAddUserWindow();});

    adminPlusLayout = new QVBoxLayout();
    adminPlusLayout->addWidget(addStudentsGroup);
    adminPlusLayout->addWidget(addStudentsToGroup);
    adminPlusLayout->addWidget(appointGroup);
    adminPlusLayout->addWidget(addUser);

    QWidget *w = new QWidget();
    w->setLayout(adminPlusLayout);
    setCentralWidget(w);
}


void MyClient::hideAdminPlusWindow()
{
    addStudentsGroup->hide();
    addStudentsToGroup->hide();
    appointGroup->hide();
    addUser->hide();
}

void MyClient::setAddGroupWindow()
{
    addGroupTitleLabel = new QLabel("Group title: ", this);
    addGroupTitle = new QLineEdit(this);
    sendGroup = new QPushButton("&Add group", this);
    addGroupGoBack = new QPushButton("&Go back", this);

    connect(sendGroup, &QPushButton::clicked, this,  [this] () {sendGroupToSystem();});
    connect(addGroupGoBack, &QPushButton::clicked, this,
            [this] () {hideAddGroupWindow(); setAdminPlusWindow();});

    QHBoxLayout *fl = new QHBoxLayout();
    fl->addWidget(addGroupTitleLabel);
    fl->addWidget(addGroupTitle);


    addGroupLayout = new QVBoxLayout();
    addGroupLayout->addLayout(fl);
    addGroupLayout->addWidget(sendGroup);
    addGroupLayout->addWidget(addGroupGoBack);

    QWidget *w = new QWidget();
    w->setLayout(addGroupLayout);
    setCentralWidget(w);
}

void MyClient::sendGroupToSystem()
{
    QString groupTitle = addGroupTitle->text();
    if(groupTitle == "")
    {
        showError("Group title is empty");
        return;
    }
    QString req = "{cmd='add group';";
    req += "groupname='" + groupTitle+"';}";\
    slotSendToServer(req);
}

void MyClient::hideAddGroupWindow()
{
    addGroupTitleLabel->hide();
    addGroupTitle->hide();
    sendGroup->hide();
    addGroupGoBack->hide();
}

void MyClient::setAddToGroupWindow()
{
    addToGroupNameLabel = new QLabel("Name:", this);
    addToGroupSurnameLabel = new QLabel("Surname:", this);
    addToGroupTitleLabel = new QLabel("Group title:", this);
    addToGroupName = new QLineEdit(this);
    addToGroupSurname = new QLineEdit(this);
    addToGroupTitle = new QLineEdit(this);
    sendToGroup = new QPushButton("&Add", this);
    addToGroupGoBack = new QPushButton("&Go back", this);

    connect(sendToGroup, &QPushButton::clicked, this,
            [this] () {sendToGroupToSystem();});
    connect(addToGroupGoBack, &QPushButton::clicked, this,
            [this] () {hideAddToGroupWindow(); setAdminPlusWindow();});

    QFormLayout *fl = new QFormLayout();
    fl->addRow(addToGroupNameLabel, addToGroupName);
    fl->addRow(addToGroupSurnameLabel, addToGroupSurname);
    fl->addRow(addToGroupTitleLabel, addToGroupTitle);

    addToGroupLayout = new QVBoxLayout();
    addToGroupLayout->addLayout(fl);
    addToGroupLayout->addWidget(sendToGroup);
    addToGroupLayout->addWidget(addToGroupGoBack);

    QWidget *w = new QWidget();
    w->setLayout(addToGroupLayout);
    setCentralWidget(w);
}

void MyClient::hideAddToGroupWindow()
{
    addToGroupNameLabel->hide();
    addToGroupSurnameLabel->hide();
    addToGroupTitleLabel->hide();
    addToGroupName->hide();
    addToGroupSurname->hide();
    addToGroupTitle->hide();
    sendToGroup->hide();
    addToGroupGoBack->hide();
}

void MyClient::sendToGroupToSystem()
{
    QString name = addToGroupName->text();
    QString surname = addToGroupSurname->text();
    QString title = addToGroupTitle->text();
    if(name == "" || surname == "" || title == "")
    {
        showError("Заполните все поля");
        return;
    }
    else
    {
        QString msg = "{cmd='add to group';";
        msg += "studentsname='" + name + "';";
        msg += "studentssurname='" + surname + "';";
        msg+= "groupname='" + title + "';}";
        slotSendToServer(msg);
    }
}

void MyClient::setAppointGroupWindow()
{
    appointGroupTeacherNameLabel = new QLabel("Teacher name:", this);
    appointGroupTeacherSurnameLabel = new QLabel("Teacher surname:", this);
    appointGroupTitleLabel = new QLabel("Group name:", this);
    appointGroupTeacherName = new QLineEdit();
    appointGroupTeacherSurname = new QLineEdit();
    appointGroupTitle = new QLineEdit();
    sendAppointGroup = new QPushButton("Appoint group", this);
    appointGroupGoBack = new QPushButton("Go back", this);

    connect(appointGroupGoBack, &QPushButton::clicked, this,
            [this] () {hideAppointGroupWindow(); setAdminPlusWindow();});
    connect(sendAppointGroup, &QPushButton::clicked, this,
            [this] () {sendAppointGroupToSystem();});

    QFormLayout *f = new QFormLayout();
    f->addRow(appointGroupTeacherNameLabel, appointGroupTeacherName);
    f->addRow(appointGroupTeacherSurnameLabel, appointGroupTeacherSurname);
    f->addRow(appointGroupTitleLabel, appointGroupTitle);

    appointGroupLayout = new QVBoxLayout();
    appointGroupLayout->addLayout(f);
    appointGroupLayout->addWidget(sendAppointGroup);
    appointGroupLayout->addWidget(appointGroupGoBack);

    QWidget *w = new QWidget();
    w->setLayout(appointGroupLayout);
    setCentralWidget(w);
}

void MyClient::hideAppointGroupWindow()
{
    appointGroupTeacherNameLabel->hide();
    appointGroupTeacherSurnameLabel->hide();
    appointGroupTitleLabel->hide();
    appointGroupTeacherName->hide();
    appointGroupTeacherSurname->hide();
    appointGroupTitle->hide();
    sendAppointGroup->hide();
    appointGroupGoBack->hide();
}

void MyClient::setAddUserWindow()
{
    addUserLoginLabel = new QLabel("Login:", this);
    addUserPasswordLabel = new QLabel("Password:", this);
    addUserNameLabel = new QLabel("Name:", this);
    addUserSurnameLabel = new QLabel("Surname:", this);
    addUserRoleLabel = new QLabel("Role:", this);

    addUserBox = new QComboBox(this);
    addUserBox->addItem("admin+");
    addUserBox->addItem("admin");
    addUserBox->addItem("teacher");
    addUserBox->addItem("student");

    addUserLogin = new QLineEdit(this);
    addUserPassword = new QLineEdit(this);
    addUserName = new QLineEdit(this);
    addUserSurname = new QLineEdit(this);
    addUserButton = new QPushButton("Add user", this);
    addUserGoBack = new QPushButton("Go back", this);

    connect(addUserGoBack, &QPushButton::clicked, this,
            [this] () {hideAddUserWindow(); setAdminPlusWindow(); });
    connect(addUserButton, &QPushButton::clicked, this,
            [this] () {sendUserToSystem();});

    addUserLayout = new QFormLayout();
    addUserLayout->addRow(addUserLoginLabel, addUserLogin);
    addUserLayout->addRow(addUserPasswordLabel, addUserPassword);
    addUserLayout->addRow(addUserNameLabel, addUserName);
    addUserLayout->addRow(addUserSurnameLabel, addUserSurname);
    addUserLayout->addRow(addUserRoleLabel, addUserBox);
    addUserLayout->addWidget(addUserButton);
    addUserLayout->addWidget(addUserGoBack);

    QWidget *w = new QWidget();
    w->setLayout(addUserLayout);
    setCentralWidget(w);
}

void MyClient::hideAddUserWindow()
{
    addUserLoginLabel->hide();
    addUserPasswordLabel->hide();
    addUserNameLabel->hide();
    addUserSurnameLabel->hide();
    addUserRoleLabel->hide();
    addUserBox->hide();
    addUserLogin->hide();
    addUserPassword->hide();
    addUserName->hide();
    addUserSurname->hide();
    addUserButton->hide();
    addUserGoBack->hide();
}

void MyClient::sendAppointGroupToSystem()
{
    QString teacherName = appointGroupTeacherName->text();
    QString teacherSurname = appointGroupTeacherSurname->text();
    QString groupName = appointGroupTitle->text();
    QString msg = "{cmd='appoint';";
    msg +="teachername='" + teacherName + "';";
    msg += "teachersurname='" + teacherSurname + "';";
    msg += "groupname='" + groupName + "';}";
    slotSendToServer(msg);
}

void MyClient::sendUserToSystem()
{
    QString msg = "{cmd='add user';";
    msg += "login='" + addUserLogin->text() + "';";
    msg += "pass='" + addUserPassword->text() + "';";
    msg += "name='" + addUserName->text() + "';";
    msg += "surname='" + addUserSurname->text() + "';";
    msg += "role='" + addUserBox->itemText(addUserBox->currentIndex()) + "';";
    msg += "}";
    slotSendToServer(msg);
}

void MyClient::setAdminWindow()
{
    adminViewResults = new QPushButton("View Results", this);
    adminViewGroups = new QPushButton("View Groups", this);
    adminViewPlannedTest = new QPushButton("View Planned Tests", this);
    adminViewGrades = new QPushButton("View Grades", this);
    adminViewTasks = new QPushButton("View Tasks", this);

    connect(adminViewResults, &QPushButton::clicked, this,
            [this] () {hideAdminWindow();
                       connect(this, SIGNAL(allResultsCollected()), this, SLOT(setViewAllResultsWindow()));
                       slotSendToServer("{cmd='view all results';}");});
    connect(adminViewGroups, &QPushButton::clicked, this,
            [this] () {hideAdminWindow();
                       connect(this, SIGNAL(allGroupsCollected()), this, SLOT(setViewAllGroupsWindow()));
                       slotSendToServer("{cmd='view all groups';}");});
     connect(this, SIGNAL(groupTeachersCollected()), this, SLOT(showGroupTeachers()));
     connect(this, SIGNAL(groupStudentsCollected()), this, SLOT(showGroupStudents()));

    adminLayout = new QVBoxLayout();
    adminLayout->addWidget(adminViewResults);
    adminLayout->addWidget(adminViewGroups);
    adminLayout->addWidget(adminViewPlannedTest);
    adminLayout->addWidget(adminViewGrades);
    adminLayout->addWidget(adminViewTasks);

    QWidget *w = new QWidget();
    w->setLayout(adminLayout);
    setCentralWidget(w);
}

void MyClient::hideAdminWindow()
{
    adminViewResults->hide();
    adminViewGroups->hide();
    adminViewPlannedTest->hide();
    adminViewGrades->hide();
    adminViewTasks->hide();
}

void MyClient::setViewAllResultsWindow()
{
    allResultsGoBack = new QPushButton("Go Back", this);

    connect(allResultsGoBack, &QPushButton::clicked, this,
            [this] () {hideViewAllResultsWindow(); setAdminWindow();});

    allResultsTable = new QTableView();
    allResultsModel = new QStandardItemModel(allResultsList.size(), 6, this);

    QList <QString> params = {"Test", "Date", "Subject", "Name", "Surname", "Percent"};

    for(int i = 0; i < 6; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        allResultsModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    for(int row = 0; row < allResultsList.size(); ++row)
    {
        for(int col = 0; col < 6; ++col)
        {
            QModelIndex index=allResultsModel->index(row,col,QModelIndex());
            allResultsModel->setData(index, allResultsList[row][col]);
        }
    }

    allResultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allResultsTable->setModel(allResultsModel);

    viewAllResultsLayout = new QVBoxLayout();
    viewAllResultsLayout->addWidget(allResultsTable);
    viewAllResultsLayout->addWidget(allResultsGoBack);

    QWidget *w = new QWidget();
    w->setLayout(viewAllResultsLayout);
    setCentralWidget(w);
}

void MyClient::hideViewAllResultsWindow()
{
    allResultsTable->hide();
    allResultsGoBack->hide();
}

void MyClient::setViewAllGroupsWindow()
{
    allGroupsGoBack = new QPushButton("Go Back", this);

    connect(allGroupsGoBack, &QPushButton::clicked, this,
            [this] () {hideViewAllGroupsWindow(); setAdminWindow();});

    allGroupsTable = new QTableView();
    allGroupsModel = new QStandardItemModel(allGroupsList.size(), 3, this);

    QList <QString> params = {"Name", "Teachers", "Students"};

    for(int i = 0; i < 3; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        allGroupsModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    for(int row = 0; row < allGroupsList.size(); ++row)
    {
        QModelIndex index=allGroupsModel->index(row,0,QModelIndex());
        allGroupsModel->setData(index, allGroupsList[row]);
    }

    allGroupsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allGroupsTable->setModel(allGroupsModel);

    for(int row = 0; row < allGroupsList.size(); ++row)
    {
        QModelIndex index1=allGroupsModel->index(row,1,QModelIndex());
        QString str = allGroupsModel->index(row,0,QModelIndex()).data().toString();
        QPushButton *button1 = new QPushButton(this);
        button1->setAccessibleName(str);
        connect(button1, &QPushButton::clicked, this,
                [this, button1] () {askGroupTeachers(button1->accessibleName());});

        allGroupsTable->setIndexWidget(index1, button1);

        QModelIndex index2=allGroupsModel->index(row,2,QModelIndex());
        QPushButton *button2 = new QPushButton(this);
        button2->setAccessibleName(str);

        connect(button2, &QPushButton::clicked, this,
                [this, button2] () {askGroupStudents(button2->accessibleName());});

        allGroupsTable->setIndexWidget(index2, button2);
    }

    viewAllGroupsLayout = new QVBoxLayout();
    viewAllGroupsLayout->addWidget(allGroupsTable);
    viewAllGroupsLayout->addWidget(allGroupsGoBack);

    QWidget *w = new QWidget();
    w->setLayout(viewAllGroupsLayout);
    setCentralWidget(w);
}

void MyClient::hideViewAllGroupsWindow()
{
    allGroupsTable->hide();
    allGroupsGoBack->hide();
}

void MyClient::askGroupTeachers(QString groupName)
{
    slotSendToServer("{cmd='view group teachers';groupname='" + groupName + "';}");
}
void MyClient::askGroupStudents(QString groupName)
{
    slotSendToServer("{cmd='view group students';groupname='" + groupName + "';}");
}
void MyClient::showGroupTeachers()
{
    QTableView *table = new QTableView(this);
    table->setAttribute( Qt::WA_DeleteOnClose );
    QStandardItemModel *model = new QStandardItemModel(groupTeachers.size(), 2, this);
    QList <QString> params = {"Name", "Surname"};
    for(int i = 0; i < 2; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < groupTeachers.size(); ++row)
    {
        for(int col = 0; col < 2; ++col)
        {
            QModelIndex index=model->index(row,col,QModelIndex());
            model->setData(index, groupTeachers[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    QDialog *d = new QDialog(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(table);
    d->setLayout(v);
    d->show();
}
void MyClient::showGroupStudents()
{
    QTableView *table = new QTableView(this);
    table->setAttribute( Qt::WA_DeleteOnClose );
    QStandardItemModel *model = new QStandardItemModel(groupStudents.size(), 2, this);
    QList <QString> params = {"Name", "Surname"};
    for(int i = 0; i < 2; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < groupStudents.size(); ++row)
    {
        for(int col = 0; col < 2; ++col)
        {
            QModelIndex index=model->index(row,col,QModelIndex());
            model->setData(index, groupStudents[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    QDialog *d = new QDialog(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(table);
    d->setLayout(v);
    d->show();
}
