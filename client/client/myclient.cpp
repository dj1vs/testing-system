#include "myclient.h"

MyClient::MyClient(const QString& strHost, int nPort, QWidget *parent)
    : QMainWindow(parent)
{
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort);

    connect(m_pTcpSocket, &QTcpSocket::connected, this, [this] {showMsg("Received the connected() signal");});
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );
    d = new QErrorMessage(this);

    this->setMinimumSize(WINW, WINH);

    connect(this, SIGNAL(groupTeachersCollected()), this, SLOT(showGroupTeachers()));
    connect(this, SIGNAL(groupStudentsCollected()), this, SLOT(showGroupStudents()));
    connect(this, SIGNAL(allResultsCollected()), this, SLOT(setViewAllResultsWindow()));
    connect(this, SIGNAL(allGroupsCollected()), this, SLOT(setViewAllGroupsWindow()));
    connect(this, SIGNAL(allPlannedTestsCollected()), this, SLOT(setViewAllPlannedTestsWindow()));
    connect(this, SIGNAL(testTasksCollected()), this, SLOT(showTestTasks()));

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

    QHBoxLayout *l1 = new QHBoxLayout();
    l1->addWidget(authorizeLoginLabel);
    l1->addWidget(authorizeLogin);

    QHBoxLayout *l2 = new QHBoxLayout();
    l2->addWidget(authorizePasswordLabel);
    l2->addWidget(authorizePassword);

    authorizeLayout = new QVBoxLayout();
    authorizeLayout->addLayout(l1);
    authorizeLayout->addLayout(l2);
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
            id = cutArg(msg, "id").toInt();
            qDebug() << id;
            hideAuthorizationWindow();
            if(role == "admin+")
                setAdminPlusWindow();
            else if(role == "admin")
                setAdminWindow();
            else if (role == "teacher")
                setTeacherWindow();
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
        QList <QString> buf = {cutArg(msg, "testname"),
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
    else if(cmd == "view all planned tests")
    {
        QString status = cutArg(msg, "status");
        if (status == "sended")
            emit allPlannedTestsCollected();
        else if (status == "started")
            allPlannedTestsList.clear();
        else
            allPlannedTestsList.push_back({cutArg(msg, "teachername"), cutArg(msg, "teachersurname"), cutArg(msg, "testname"),
                                          cutArg(msg, "subject"), cutArg(msg, "date")});
    }
    else if(cmd == "view test tasks")
    {
        QString status = cutArg(msg, "status");
        //QList <QString> params = {"testname", "task", "answeroptions","answer","theme"};
        if(status == "sended")
            emit testTasksCollected();
        else if(status == "started")
            allPlannedTestsTaskList.clear();
        else
            allPlannedTestsTaskList.push_back({cutArg(msg, "testname"), cutArg(msg, "taskname"), cutArg(msg, "answeroptions"),
                                              cutArg(msg, "answertext"), cutArg(msg, "theme")});
    }
    else if(cmd == "add task")
    {
        QString status = cutArg(msg, "status");
        if (status == "sended")
        {
            showMsg("task added successfully");
            addTaskAnswer->clear();
            addTaskQuesition->clear();
            addTaskAnswerOptions.clear();
            addTaskAnswerOptionsModel->setStringList(addTaskAnswerOptions);
            addTaskAnswerOptionsView->setModel(addTaskAnswerOptionsModel);
        }
    }
    else if(cmd == "validate tasks amount")
    {
        QString status = cutArg(msg, "status");
        bool t = (status == "success");
        if (!t)
        {
            quint16 amount = cutArg(msg, "tasksamount").toInt();
            if(!amount)
                return;
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "", "There is no enough tasks. Current tasks amount:" + QString::number(amount),
                                            QMessageBox::Yes|QMessageBox::No);
            if(reply == QMessageBox::Yes)
                t = true;
        }
        qDebug() << t;

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

    connect(adminViewResults, &QPushButton::clicked, this,
            [this] () {hideAdminWindow();
                       slotSendToServer("{cmd='view all results';}");});
    connect(adminViewGroups, &QPushButton::clicked, this,
            [this] () {hideAdminWindow();
                       slotSendToServer("{cmd='view all groups';}");});
    connect(adminViewPlannedTest, &QPushButton::clicked, this,
            [this] () {hideAdminWindow();
                       slotSendToServer("{cmd='view all planned tests';}");});

    adminLayout = new QVBoxLayout();
    adminLayout->addWidget(adminViewResults);
    adminLayout->addWidget(adminViewGroups);
    adminLayout->addWidget(adminViewPlannedTest);

    QWidget *w = new QWidget();
    w->setLayout(adminLayout);
    setCentralWidget(w);
}

void MyClient::hideAdminWindow()
{
    adminViewResults->hide();
    adminViewGroups->hide();
    adminViewPlannedTest->hide();
}

void MyClient::setViewAllResultsWindow()
{
    allResultsGoBack = new QPushButton("Go Back", this);
    sortAllResults = new QPushButton("Редактировать параметры сортировки", this);

    connect(allResultsGoBack, &QPushButton::clicked, this,
            [this] () {hideViewAllResultsWindow(); setAdminWindow();});
    connect(sortAllResults, &QPushButton::clicked, this,
            [this] () {showAllResultsSort();});

    allResultsTable = new QTableView();
    allResultsModel = new QStandardItemModel(allResultsList.size(), 5, this);

    QList <QString> params = {"Test", "Subject", "Name", "Surname", "Percent"};

    for(int i = 0; i < 5; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        allResultsModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    for(int row = 0; row < allResultsList.size(); ++row)
    {
        for(int col = 0; col < 5; ++col)
        {
            QModelIndex index=allResultsModel->index(row,col,QModelIndex());
            allResultsModel->setData(index, allResultsList[row][col]);
        }
    }

    allResultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allResultsTable->setModel(allResultsModel);

    viewAllResultsLayout = new QVBoxLayout();
    viewAllResultsLayout->addWidget(allResultsTable);
    viewAllResultsLayout->addWidget(sortAllResults);
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

void MyClient::showAllResultsSort()
{
    allResultsSortNameLabel = new QLabel("Name:", this);
    allResultsSortSurnameLabel = new QLabel("Surname:", this);
    allResultsSortSubjectLabel = new QLabel("Subject:", this);
    allResultsSortTestLabel = new QLabel("Test:", this);

    allResultsSortName = new QLineEdit(this);
    allResultsSortSurname = new QLineEdit(this);
    allResultsSortSubject = new QLineEdit(this);
    allResultsSortTest = new QLineEdit(this);

    allResultsSortSave = new QPushButton("Save", this);

    QHBoxLayout *name = new QHBoxLayout();
    name->addWidget(allResultsSortNameLabel);
    name->addWidget(allResultsSortName);
    QHBoxLayout *surname = new QHBoxLayout();
    surname->addWidget(allResultsSortSurnameLabel);
    surname->addWidget(allResultsSortSurname);
    QHBoxLayout *subject = new QHBoxLayout();
    subject->addWidget(allResultsSortSubjectLabel);
    subject->addWidget(allResultsSortSubject);
    QHBoxLayout *test = new QHBoxLayout();
    test->addWidget(allResultsSortTestLabel);
    test->addWidget(allResultsSortTest);

    allResultsSortLayout = new QVBoxLayout();
    allResultsSortLayout->addLayout(name);
    allResultsSortLayout->addLayout(surname);
    allResultsSortLayout->addLayout(subject);
    allResultsSortLayout->addLayout(test);
    allResultsSortLayout->addWidget(allResultsSortSave);

    QDialog *d = new QDialog(this);
    d->setLayout(allResultsSortLayout);
    d->show();

    connect(allResultsSortSave, &QPushButton::clicked, this,
            [this, d] () {editAllResultsTable(); d->close();});
}

void MyClient::editAllResultsTable()
{
    QString name = allResultsSortName->text();
    QString surname = allResultsSortSurname->text();
    QString subject = allResultsSortSubject->text();
    QString test = allResultsSortTest->text();

    if(name == "" && surname == "" && subject == "" && test == "")
    {
        setViewAllResultsWindow();
        return;
    }

    //QList <QString> params = {"Test", "Subject", "Name", "Surname", "Percent"};

    for(int row = 0; row < allResultsModel->rowCount(); ++row)
    {
        QString modelName = allResultsModel->index(row,2,QModelIndex()).data().toString();
        QString modelSurname = allResultsModel->index(row,3,QModelIndex()).data().toString();
        QString modelSubject = allResultsModel->index(row,1,QModelIndex()).data().toString();
        QString modelTest = allResultsModel->index(row,0,QModelIndex()).data().toString();

        if ((name != "" && surname != "" && (modelName != name || modelSurname != surname))
                || (subject != "" && modelSubject != subject) || (test != "" && modelTest != test))
        {
            allResultsModel->removeRow(row);
            row--;
        }
    }
    allResultsTable->setModel(allResultsModel);
}

void MyClient::setViewAllPlannedTestsWindow()
{
    //    QTableView *allPlannedTestsTable;
    //    QStandardItemModel *allPlannedTestsModel;
    //    QPushButton *allPlannedTestsGoBack;
    //    QVBoxLayout *allPlannedTestsLayout;
    allPlannedTestsTable = new QTableView(this);
    allPlannedTestsTable->setAttribute(Qt::WA_DeleteOnClose);
    allPlannedTestsModel = new QStandardItemModel(allPlannedTestsList.size(), allPlannedTestsList[0].size() + 1, this);
    QList <QString> params = {"Teacher name", "Teacher surname", "Test", "Subject", "Date", "Tasks"};
    for(int i = 0; i < allPlannedTestsList[0].size()+1; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        allPlannedTestsModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < allPlannedTestsList.size(); ++row)
    {
        for(int col = 0; col < allPlannedTestsList[0].size(); ++col)
        {
            QModelIndex index=allPlannedTestsModel->index(row,col,QModelIndex());
            allPlannedTestsModel->setData(index, allPlannedTestsList[row][col]);
        }
    }
    allPlannedTestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allPlannedTestsTable->setModel(allPlannedTestsModel);

    for(int row = 0; row < allPlannedTestsList.size(); ++row)
    {
        QModelIndex index=allPlannedTestsModel->index(row,allPlannedTestsList[0].size(),QModelIndex());
        QPushButton *button = new QPushButton();
        button->setAccessibleName(allPlannedTestsModel->index(row,2,QModelIndex()).data().toString());
        connect(button, &QPushButton::clicked, this,
                [this, button] {slotSendToServer("{cmd='view test tasks';testname='" + button->accessibleName() + "';}");});
        allPlannedTestsTable->setIndexWidget(index, button);
    }

    allPlannedTestsGoBack = new QPushButton("Go back");
    allPlannedTestsSort = new QPushButton("Sort");
    allPlannedTestsGoBack->setAttribute(Qt::WA_DeleteOnClose);

    connect(allPlannedTestsGoBack, &QPushButton::clicked, this,
            [this] {hideViewAllPlannedTestsWindow(); setAdminWindow();});

    connect(allPlannedTestsSort, SIGNAL(clicked()), this, SLOT(showAllPlannedTestsSort()));
    allPlannedTestsLayout = new QVBoxLayout();
    allPlannedTestsLayout->addWidget(allPlannedTestsTable);
    allPlannedTestsLayout->addWidget(allPlannedTestsSort);
    allPlannedTestsLayout->addWidget(allPlannedTestsGoBack);

    QWidget *w = new QWidget();
    w->setLayout(allPlannedTestsLayout);
    setCentralWidget(w);

}
void MyClient::hideViewAllPlannedTestsWindow()
{
    allPlannedTestsTable->close();
    allPlannedTestsGoBack->close();
}

void MyClient::showAllPlannedTestsSort()
{
    allPlannedTestsSortNameLabel = new QLabel("Teacher name:", this);
    allPlannedTestsSortSurnameLabel = new QLabel("Teacher surname:", this);
    allPlannedTestsSortTestLabel = new QLabel("Test name:", this);
    allPlannedTestsSortDateLabel = new QLabel("Date (dd.MM.yyyy):", this);
    allPlannedTestsSortSubjectLabel = new QLabel("Subject:", this);
    allPlannedTestsSortName = new QLineEdit();
    allPlannedTestsSortSurname = new QLineEdit();
    allPlannedTestsSortTest = new QLineEdit();
    allPlannedTestsSortDate = new QDateEdit();
    allPlannedTestsSortSubject = new QLineEdit();
    allPlannedTestsSortViewPast = new QRadioButton("Прошедшие");
    allPlannedTestsSortViewFuture= new QRadioButton("Запланированные");
    allPlannedTestsSortViewAll= new QRadioButton("Все");
    allPlannedTestsSortSave = new QPushButton("Сохранить ");

    QHBoxLayout *name = new QHBoxLayout();
    name->addWidget(allPlannedTestsSortNameLabel);
    name->addWidget(allPlannedTestsSortName);

    QHBoxLayout *surname = new QHBoxLayout();
    surname->addWidget(allPlannedTestsSortSurnameLabel);
    surname->addWidget(allPlannedTestsSortSurname);

    QHBoxLayout *test = new QHBoxLayout();
    test->addWidget(allPlannedTestsSortTestLabel);
    test->addWidget(allPlannedTestsSortTest);

    QHBoxLayout *date = new QHBoxLayout();
    date->addWidget(allPlannedTestsSortDateLabel);
    date->addWidget(allPlannedTestsSortDate);

    QHBoxLayout *subject = new QHBoxLayout();
    subject->addWidget(allPlannedTestsSortSubjectLabel);
    subject->addWidget(allPlannedTestsSortSubject);


    QHBoxLayout *options = new QHBoxLayout();
    options->addWidget(allPlannedTestsSortViewPast);
    options->addWidget(allPlannedTestsSortViewFuture);
    options->addWidget(allPlannedTestsSortViewAll);


    allPlannedTestsSortLayout = new QVBoxLayout();
    allPlannedTestsSortLayout->addLayout(name);
    allPlannedTestsSortLayout->addLayout(surname);
    allPlannedTestsSortLayout->addLayout(test);
    allPlannedTestsSortLayout->addLayout(date);
    allPlannedTestsSortLayout->addLayout(subject);
    allPlannedTestsSortLayout->addLayout(options);
    allPlannedTestsSortLayout->addWidget(allPlannedTestsSortSave);

    QDialog *d = new QDialog();
    connect(allPlannedTestsSortSave, &QPushButton::clicked, this, [this, d] {editAllPlannedTestsTable(); d->close();});

    d->setLayout(allPlannedTestsSortLayout);
    d->show();

}
void MyClient::editAllPlannedTestsTable()
{
    setViewAllPlannedTestsWindow();
    QString name = allPlannedTestsSortName->text();
    QString surname = allPlannedTestsSortSurname->text();
    QString test = allPlannedTestsSortTest->text();
    QDate d = allPlannedTestsSortDate->date();
    QString date = QString::number(d.year()) + (d.month() < 10 ? "-0" : "-") + QString::number(d.month()) + (d.day() < 10 ? "-s0" : "-") + QString::number(d.day());
    QString subject = allPlannedTestsSortSubject->text();
    bool isPast = allPlannedTestsSortViewPast->isChecked();
    bool isFuture = allPlannedTestsSortViewFuture->isChecked();
    bool isAll = allPlannedTestsSortViewAll->isChecked();

    if(name == "" && surname == "" && subject == "" && test == "" && date == "2000-01-01" && !isPast  && !isFuture && !isAll)
        return;

    //QList <QString> params = {"Teacher name", "Teacher surname", "Test", "Subject", "Date"};

    for(int row = 0; row < allPlannedTestsModel->rowCount(); ++row)
    {
        QString modelName = allPlannedTestsModel->index(row,0,QModelIndex()).data().toString();
        QString modelSurname = allPlannedTestsModel->index(row,1,QModelIndex()).data().toString();
        QString modelSubject = allPlannedTestsModel->index(row,3,QModelIndex()).data().toString();
        QString modelTest = allPlannedTestsModel->index(row,2,QModelIndex()).data().toString();
        QString modelDate = allPlannedTestsModel->index(row,4,QModelIndex()).data().toString();
        qDebug() << modelDate << date;

        if ( (name != "" &&  surname != "" && (modelName != name || modelSurname != surname)) || (test != "" && modelTest != test)
             || (date != "2000-01-01" && date != modelDate) || (subject != "" && subject != modelSubject) ||
             (isPast && QDate::fromString(modelDate) >= QDate::currentDate()) || (isFuture && QDate::fromString(modelDate) < QDate::currentDate()))
        {
            allPlannedTestsModel->removeRow(row);
            row--;
        }

    }
    allPlannedTestsTable->setModel(allPlannedTestsModel);
}
void MyClient::showTestTasks()
{
//    QList <QList<QString>> allPlannedTestsTaskList;
//    quint16 tasksAmount = 0;
//    quint16 currTask = 0;
//    QPushButton *allPlannedTestsTaskNext;
//    QPushButton *allPlannedTestsTaskPrev;
//    QTextBrowser *allPlannedTestsTaskText;
//    QListView *allPlannedTestsTaskAnswerOptions;

    if (!allPlannedTestsTaskList.size())
    {
        showError("Current test is empty");
        return;
    }

    tasksAmount = allPlannedTestsTaskList.size();
    currTask = 0;
    allPlannedTestsTaskNext = new QPushButton("next");
    allPlannedTestsTaskPrev = new QPushButton("prev");

    connect(allPlannedTestsTaskNext, SIGNAL(clicked()), this, SLOT(showNextTask()));
    connect(allPlannedTestsTaskPrev, SIGNAL(clicked()), this, SLOT(showPrevTask()));
    allPlannedTestsTaskText = new QTextBrowser();
    allPlannedTestsTaskAnswer = new QTextBrowser();
    allPlannedTestsTaskText->setText(allPlannedTestsTaskList[0][1]);
    allPlannedTestsTaskAnswer->setText(allPlannedTestsTaskList[0][3]);
    allPlannedTestsTaskAnswerOptionsModel = new QStringListModel();
    allPlannedTestsTaskAnswerOptionsModel->setStringList(allPlannedTestsTaskList[0][2].split(';'));
    allPlannedTestsTaskAnswerOptionsView = new QListView();
    allPlannedTestsTaskAnswerOptionsView->setModel(allPlannedTestsTaskAnswerOptionsModel);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(allPlannedTestsTaskPrev);
    buttons->addWidget(allPlannedTestsTaskNext);


    allPlannedTestsTaskLayout = new QVBoxLayout();
    allPlannedTestsTaskLayout->addWidget(allPlannedTestsTaskText);
    allPlannedTestsTaskLayout->addWidget(allPlannedTestsTaskAnswerOptionsView);
    allPlannedTestsTaskLayout->addWidget(allPlannedTestsTaskAnswer);
    allPlannedTestsTaskLayout->addLayout(buttons);

    QDialog *d = new QDialog(this);
    d->setLayout(allPlannedTestsTaskLayout);
    d->show();

}

void MyClient::showNextTask()
{
    if(currTask+1 >= allPlannedTestsTaskList.size())
        return;
    ++currTask;
    allPlannedTestsTaskText->setText(allPlannedTestsTaskList[currTask][1]);
    allPlannedTestsTaskAnswer->setText(allPlannedTestsTaskList[currTask][3]);
    allPlannedTestsTaskAnswerOptionsModel->setStringList(allPlannedTestsTaskList[currTask][2].split(';'));
    allPlannedTestsTaskAnswerOptionsView->setModel(allPlannedTestsTaskAnswerOptionsModel);
}
void MyClient::showPrevTask()
{
    if(!currTask)
        return;
    --currTask;
    allPlannedTestsTaskText->setText(allPlannedTestsTaskList[currTask][1]);
    allPlannedTestsTaskAnswer->setText(allPlannedTestsTaskList[currTask][3]);
    allPlannedTestsTaskAnswerOptionsModel->setStringList(allPlannedTestsTaskList[currTask][2].split(';'));
    allPlannedTestsTaskAnswerOptionsView->setModel(allPlannedTestsTaskAnswerOptionsModel);
}

void MyClient::setTeacherWindow()
{
    newTaskButton = new QPushButton("new task");
    newTestButton = new QPushButton("new test");
    viewTeacherGroupsButton = new QPushButton("view groups");
    viewTeacherResultsButton = new QPushButton("view results");

    connect(newTaskButton, &QPushButton::clicked, this,
            [this] {hideTeacherWindow(); setAddTaskWindow();});
    connect(newTestButton, &QPushButton::clicked, this,
            [this] {hideTeacherWindow(); setAddTestWindow();});

    teacherWindowLayout = new QVBoxLayout();
    teacherWindowLayout->addWidget(newTaskButton);
    teacherWindowLayout->addWidget(newTestButton);
    teacherWindowLayout->addWidget(viewTeacherGroupsButton);
    teacherWindowLayout->addWidget(viewTeacherResultsButton);

    QWidget *w = new QWidget();
    w->setLayout(teacherWindowLayout);
    setCentralWidget(w);
}
void MyClient::hideTeacherWindow()
{
    newTaskButton->close();
    newTestButton->close();
    viewTeacherGroupsButton->close();
    viewTeacherResultsButton->close();
}

void MyClient::setAddTaskWindow()
{
    addTaskAnswerOptions.clear();
    addTaskQuesitionLabel = new QLabel("Вопрос:");
    addTaskAnswerLabel = new QLabel("Answer");
    addTaskAnswerOptionsLabel = new QLabel("Answer options");
    addTaskThemeLabel = new QLabel("Theme");
    addTaskSubjectLabel = new QLabel("Subject");

    addTaskSave = new QPushButton("save");
    addTaskQuit = new QPushButton("quit");
    addTaskNewOption = new QPushButton("add new answer option");
    addTaskDeleteEmpty = new QPushButton("delete empty");

    connect(addTaskQuit, &QPushButton::clicked, this,
            [this] {hideAddTaskWindow(); setTeacherWindow();});
    connect(addTaskSave, SIGNAL(clicked()), this, SLOT(sendTaskToSystem()));

    connect(addTaskNewOption, &QPushButton::clicked, this,
            [this] {showAddAnswerOptions();});

    connect(addTaskDeleteEmpty, &QPushButton::clicked, this,
            [this] {addTaskAnswerOptionsDeleteEmpty();});

    addTaskQuesition = new QTextEdit();
    addTaskAnswer = new QTextEdit();

    addTaskAnswerOptionsModel = new QStringListModel();
    addTaskAnswerOptionsView = new QListView();

    addTaskTheme = new QLineEdit();
    addTaskSubject = new QLineEdit();

    addTaskLayout = new QVBoxLayout();
    addTaskLayout->addWidget(addTaskQuesitionLabel);
    addTaskLayout->addWidget(addTaskQuesition);
    addTaskLayout->addWidget(addTaskAnswerOptionsLabel);
    addTaskLayout->addWidget(addTaskAnswerOptionsView);
    addTaskLayout->addWidget(addTaskDeleteEmpty);
    addTaskLayout->addWidget(addTaskNewOption);
    addTaskLayout->addWidget(addTaskAnswerLabel);
    addTaskLayout->addWidget(addTaskAnswer);
    addTaskLayout->addWidget(addTaskSubjectLabel);
    addTaskLayout->addWidget(addTaskSubject);
    addTaskLayout->addWidget(addTaskThemeLabel);
    addTaskLayout->addWidget(addTaskTheme);
    addTaskLayout->addWidget(addTaskSave);
    addTaskLayout->addWidget(addTaskQuit);

    QWidget *w = new QWidget();
    w->setLayout(addTaskLayout);
    setCentralWidget(w);
}
void MyClient::hideAddTaskWindow()
{
    addTaskQuesitionLabel->close();
    addTaskQuesition->close();
    addTaskAnswerOptionsLabel->close();
    addTaskAnswerOptionsView->close();
    addTaskNewOption->close();
    addTaskAnswerLabel->close();
    addTaskAnswer->close();
    addTaskSubjectLabel->close();
    addTaskSubject->close();
    addTaskThemeLabel->close();
    addTaskTheme->close();
    addTaskSave->close();
    addTaskQuit->close();
}

void MyClient::sendTaskToSystem()
{
    QString task = addTaskQuesition->toPlainText();
    QString answer = addTaskAnswer->toPlainText();
    QList <QString> answerOptions = addTaskAnswerOptionsModel->stringList();
    QString theme = addTaskTheme->text();
    QString subject = addTaskSubject->text();

    if (task == "" || answer == "" || theme == "" || subject == "" || answerOptions.isEmpty())
    {
        showError("Заполните все поля.");
        return;
    }
    else
    {
        QString msg = "{cmd='add task';id='" + QString::number(id) + "';tasktext='" + task + "';";
        msg += "answer='" + answer + "';theme='" + theme + "';subject='" + subject + "';answerOptions='";
        for(auto &i : answerOptions)
            msg += i + ';';
        msg += "';}";
        qDebug() << msg;
        slotSendToServer(msg);
    }
}

void MyClient::showAddAnswerOptions()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New answer option:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        addTaskAnswerOptions.push_back(text);
        addTaskAnswerOptionsModel->setStringList(addTaskAnswerOptions);
        addTaskAnswerOptionsView->setModel(addTaskAnswerOptionsModel);
    }
}
void MyClient::addTaskAnswerOptionsDeleteEmpty()
{
    for(int i = 0; i < addTaskAnswerOptionsModel->rowCount(); ++i)
    {
        if(addTaskAnswerOptionsModel->stringList().at(i) == "")
            addTaskAnswerOptionsModel->removeRow(i);
    }
    addTaskAnswerOptionsView->setModel(addTaskAnswerOptionsModel);
}
void MyClient::setAddTestWindow()
{
    addTestQuit = new QPushButton("quit");
    addTestGoRandom = new QPushButton("random");
    addTestGoManual = new QPushButton("manual");
    connect(addTestQuit, &QPushButton::clicked, this, [this] {hideAddTestWindow(); setTeacherWindow();});
    connect(addTestGoRandom, &QPushButton::clicked, this, [this] {hideAddTestWindow(); setAddTestRandomWindow();});
    connect(addTestGoManual, &QPushButton::clicked, this, [this] {hideAddTestWindow(); setAddTestManualWindow();});
    addTestLayout = new QVBoxLayout();
    addTestLayout->addWidget(addTestGoRandom);
    addTestLayout->addWidget(addTestGoManual);
    addTestLayout->addWidget(addTestQuit);

    QWidget *w = new QWidget();
    w->setLayout(addTestLayout);
    setCentralWidget(w);
}
void MyClient::hideAddTestWindow()
{
    disconnect(addTestGoRandom);
    disconnect(addTestGoManual);
    disconnect(addTestQuit);
    addTestQuit->close();
    addTestGoRandom->close();
    addTestGoManual->close();
}

void MyClient::setAddTestRandomWindow()
{
    addTestRandomNameLabel = new QLabel("name");
    addTestRandomThemeLabel = new QLabel("Theme");
    addTestRandomSubjectLabel = new QLabel("Subject");
    addTestRandomAuthorLabel = new QLabel("Author");
    addTestRandomAmountLabel = new QLabel("Amount(1-999)");
    addTestRandomDateLabel = new QLabel("date");
    addTestRandomDate = new QDateEdit();

    addTestRandomTheme = new QLineEdit();
    addTestRandomSubject = new QLineEdit();
    addTestRandomName = new QLineEdit();

    addTestRandomMine = new QRadioButton("Mine");
    addTestRandomAll = new QRadioButton("All");

    QHBoxLayout *date = new QHBoxLayout();
    date->addWidget(addTestRandomDateLabel);
    date->addWidget(addTestRandomDate);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(addTestRandomMine);
    buttons->addWidget(addTestRandomAll);

    addTestRandomAmount = new QSpinBox();
    addTestRandomAmount->setMinimum(1);
    addTestRandomAmount->setMaximum(999);
    addTestRandomAmount->setValue(3);

    addTestRandomQuit = new QPushButton("quit");
    addTestRandomSave =new QPushButton("save");

    connect(addTestRandomQuit, &QPushButton::clicked, this,
            [this] {hideAddTestRandomWindow(); setAddTestWindow();});
    connect(addTestRandomSave, &QPushButton::clicked, this,
            [this] {
        if (addTestRandomTheme->text() == "" || addTestRandomSubject->text() == "" ||
                addTestRandomName->text() == "" || (!addTestRandomMine->isChecked() && !addTestRandomAll->isChecked())
                || addTestRandomDate->date() < QDate::currentDate())
        {
            showError("fill all");
            return;
        }
        else
        {
            QString msg = "{cmd='validate tasks amount';teacherid='" + QString::number(id) + "';";
            msg += "theme='"  + addTestRandomTheme->text() + "';";
            msg += "subject='" + addTestRandomSubject->text() + "';";
            msg += "testsize='" + QString::number(addTestRandomAmount->value()) + "';";
            msg += "author='" + QString((addTestRandomMine->isChecked() ? "ME" : "ALL")) + "';";
            msg += '}';
            qDebug() << msg;
            slotSendToServer(msg);
        }
    });

    addTestRandomLayout = new QVBoxLayout();
    addTestRandomLayout->addWidget(addTestRandomNameLabel);
    addTestRandomLayout->addWidget(addTestRandomName);
    addTestRandomLayout->addWidget(addTestRandomThemeLabel);
    addTestRandomLayout->addWidget(addTestRandomTheme);
    addTestRandomLayout->addWidget(addTestRandomSubjectLabel);
    addTestRandomLayout->addWidget(addTestRandomSubject);
    addTestRandomLayout->addWidget(addTestRandomAuthorLabel);
    addTestRandomLayout->addLayout(buttons);
    addTestRandomLayout->addLayout(date);
    addTestRandomLayout->addWidget(addTestRandomAmountLabel);
    addTestRandomLayout->addWidget(addTestRandomAmount);
    addTestRandomLayout->addWidget(addTestRandomSave);
    addTestRandomLayout->addWidget(addTestRandomQuit);

    QWidget *w = new QWidget();
    w->setLayout(addTestRandomLayout);
    setCentralWidget(w);
}
void MyClient::hideAddTestRandomWindow()
{
    disconnect(addTestRandomQuit);
    addTestRandomTheme->close();
    addTestRandomSubject->close();
    addTestRandomMine->close();
    addTestRandomAll->close();
    addTestRandomAmount->close();
    addTestRandomQuit->close();
}
void MyClient::setAddTestManualWindow()
{

}
void MyClient::hideAddTestManualWindow()
{

}
