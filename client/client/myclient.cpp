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

    this->resize(WINW, WINH);

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
    qDebug() << "Received the connected() signal";
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
            qDebug() << "Succesfuly logged in!";
            qDebug() << "Role: " + role;
            hideAuthorizationWindow();
            if(role == "admin+")
                setAdminPlusWindow();
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
                qDebug() << "Group added succesfully";
                addGroupTeacherName->setText("");
                addGroupTeacherSurname->setText("");
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
            qDebug() << "added to group succesfully";
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
    connect(addStudentsToGroup, &QPushButton::clicked, this, [this] () {hideAdminPlusWindow(); setAddToGroupWindow();});

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
    addGroupTeacherNameLabel = new QLabel("Teacher name: ", this);
    addGroupTeacherSurnameLabel = new QLabel("Teacher surname: ", this);
    addGroupTitleLabel = new QLabel("Group title: ", this);
    addGroupTeacherName = new QLineEdit(this);
    addGroupTeacherSurname = new QLineEdit(this);
    addGroupTitle = new QLineEdit(this);
    sendGroup = new QPushButton("&Add group", this);
    addGroupGoBack = new QPushButton("&Go back", this);

    connect(sendGroup, &QPushButton::clicked, this,  [this] () {sendGroupToSystem();});
    connect(addGroupGoBack, &QPushButton::clicked, this,
            [this] () {hideAddGroupWindow(); setAdminPlusWindow();});

    QFormLayout *fl = new QFormLayout();
    fl->addRow(addGroupTeacherNameLabel, addGroupTeacherName);
    fl->addRow(addGroupTeacherSurnameLabel, addGroupTeacherSurname);
    fl->addRow(addGroupTitleLabel, addGroupTitle);


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
    QString teacherName = addGroupTeacherName->text();
    QString teacherSurname = addGroupTeacherSurname->text();
    QString groupTitle = addGroupTitle->text();
    if(groupTitle == "")
    {
        showError("Group title is empty");
        return;
    }
    QString req = "{cmd='add group';";
    req += "teachername='"+teacherName+"';";
    req += "teachersurname='"+teacherSurname+"';";
    req += "groupname='" + groupTitle+"';}";\
    slotSendToServer(req);
}

void MyClient::hideAddGroupWindow()
{
    addGroupTeacherNameLabel->hide();
    addGroupTeacherSurnameLabel->hide();
    addGroupTitleLabel->hide();
    addGroupTeacherName->hide();
    addGroupTeacherSurname->hide();
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
