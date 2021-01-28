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

    setMinimumSize(WINW, WINH);

    setAuthorizationWindow();
}

MyClient::~MyClient()
{
}

void MyClient::setAuthorizationWindow()
{
    aw = new AuthorizationWidget(this);
    connect(aw->authorize, &QPushButton::clicked, this, [this]
    {
        slotSendToServer("{cmd='authorize';login='" + aw->getLogin() + "';"
                         "pass='" + aw->getPassword() + "';}");
    });
    setCentralWidget(aw);
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
            delete aw;
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
        if(!r)
            showMsg("Group added succesfully");
    }
    else if(cmd == "add to group")
    {
        int r = cutArg(msg, "status").toInt();
        switch(r)
        {
        case 0:
            showMsg("added to group succesfully");
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
            delete adminW;
            arw = new AllResultsWidget(this, allResultsList);
            connect(arw->goBack, &QPushButton::clicked, this, [this] {delete arw; setAdminWindow();});
            setCentralWidget(arw);
            return;
        }
        else if(cutArg(msg, "status") == "started")
        {
            allResultsList.clear();
            return;
        }
        allResultsList.push_back({
        cutArg(msg, "testname"), cutArg(msg, "subject"),
        cutArg(msg, "studentsname"), cutArg(msg, "studentssurname"),
        cutArg(msg, "percent")});
    }
    else if(cmd == "view all groups")
    {
        if(cutArg(msg, "status") == "sended")
        {
            QTableView *table = new QTableView();
            QStandardItemModel *model = new QStandardItemModel(allGroupsList.size(), 3, this);

            QList <QString> params = {"Name", "Teachers", "Students"};

            for(int i = 0; i < 3; ++i)
            {
                QByteArray ba = params[i].toLocal8Bit();
                const char* c_str = ba.data();
                model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
            }

            for(int row = 0; row < allGroupsList.size(); ++row)
            {
                QModelIndex index=model->index(row,0,QModelIndex());
                model->setData(index, allGroupsList[row]);
            }

            table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            table->setModel(model);

            for(int row = 0; row < allGroupsList.size(); ++row)
            {
                QModelIndex index1=model->index(row,1,QModelIndex());
                QString str = model->index(row,0,QModelIndex()).data().toString();
                QPushButton *button1 = new QPushButton(this);
                button1->setAccessibleName(str);
                connect(button1, &QPushButton::clicked, this,
                        [this, button1] () {slotSendToServer("{cmd='view group teachers';groupname='" + button1->accessibleName() + "';}");});

                table->setIndexWidget(index1, button1);

                QModelIndex index2=model->index(row,2,QModelIndex());
                QPushButton *button2 = new QPushButton(this);
                button2->setAccessibleName(str);

                connect(button2, &QPushButton::clicked, this,
                        [this, button2] () {slotSendToServer("{cmd='view group students';groupname='" + button2->accessibleName() + "';}");});


                table->setIndexWidget(index2, button2);
            }

            allGroupsW = new AllGroupsWidget(this, table, model);
            connect(allGroupsW->goBack, &QPushButton::clicked, this, [this]
               {
               delete allGroupsW;
               groupStudents.clear(); groupTeachers.clear();
               allGroupsList.clear();
               setAdminWindow();});
            setCentralWidget(allGroupsW);
            return;
        }
        allGroupsList.push_back(cutArg(msg, "name"));
    }
    else if (cmd == "view group teachers")
    {
        QString status = cutArg(msg, "status");
        if(status == "sended")
        {
            allGroupsW->showGroupTeachers(groupTeachers);
            groupTeachers.clear();
        }
        else
            groupTeachers.push_back({cutArg(msg, "name"), cutArg(msg, "surname")});
    }
    else if (cmd == "view group students")
    {
        QString status = cutArg(msg, "status");
        if(status == "sended")
        {
            allGroupsW->showGroupStudents(groupStudents);
            groupStudents.clear();
        }
        else
            groupStudents.push_back({cutArg(msg, "name"), cutArg(msg, "surname")});
    }
    else if(cmd == "view all planned tests")
    {
        QString status = cutArg(msg, "status");
        if (status == "sended")
        {
            atw = new AllTestsWidget(this, allPlannedTestsList);
            connect(atw->goBack, &QPushButton::clicked, this,
                    [this] {delete atw; setAdminWindow();});
            for(int i = 0; i < atw->buttons.size(); ++i)
            {
                connect(atw->buttons[i], &QPushButton::clicked, this, [this, i]
                {slotSendToServer("{cmd='view test tasks';testname='" + atw->buttons[i]->accessibleName() + "';}");});
            }
            setCentralWidget(atw);
        }
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
           atw->showTestTasks(allPlannedTestsTaskList);
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
            showMsg("task added successfully");
    }
    else if(cmd == "validate tasks amount")
    {

    }
    else if(cmd == "add test")
        showMsg("added!");
    else if(cmd == "get tasks")
    {
    }
}

QString MyClient::cutArg(QString str, QString cmd)
{
    int pos1 = str.indexOf(cmd) + cmd.size() + 2;
    int pos2 = str.midRef(pos1, str.size()).indexOf("'");
    return str.mid(pos1, pos2);
}

void MyClient::setAdminPlusWindow()
{
    adminPlusW = new AdminPlusWidget(this);

    connect(adminPlusW->addGroup, &QPushButton::clicked, this, [this] {
        delete adminPlusW;
        agw = new AddGroupWidget(this);

        connect(agw->sendGroup, &QPushButton::clicked, this, [this] {

            QString groupTitle = agw->getGroupTitle();
            if(groupTitle == "")
            {
                showError("Group title is empty");
                return;
            }
            QString req = "{cmd='add group';";
            req += "groupname='" + groupTitle+"';}";\
            slotSendToServer(req);
        });

        connect(agw->goBack, &QPushButton::clicked, this, [this] {delete agw; setAdminPlusWindow();});
        setCentralWidget(agw);});


    connect(adminPlusW->addToGroup, &QPushButton::clicked, this, [this] {
        delete adminPlusW;
        atgw = new AddToGroupWidget(this);
        connect(atgw->sendToGroup, &QPushButton::clicked, this,
                [this] () {
            QString name = atgw->getName();
            QString surname = atgw->getSurame();
            QString title = atgw->getTitle();
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
        });
        connect(atgw->goBack, &QPushButton::clicked, this,
                [this] () {delete atgw; setAdminPlusWindow();});
        setCentralWidget(atgw);});


    connect(adminPlusW->appointGroup, &QPushButton::clicked, this, [this] {
        delete adminPlusW;
        appgw = new AppointGroupWidget(this);
        connect(appgw->goBack, &QPushButton::clicked, this,
                [this] () {delete appgw; setAdminPlusWindow();});

        connect(appgw->sendAppointGroup, &QPushButton::clicked, this,
                [this] () {
            slotSendToServer("{cmd='appoint';"
            "teachername='" + appgw->getName() + "';"
            "teachersurname='" + appgw->getSurname() + "';"
            "groupname='" + appgw->getTitle() + "';}");
        });

        setCentralWidget(appgw);});


    connect(adminPlusW->addUser, &QPushButton::clicked, this, [this] {
        delete adminPlusW;  auw = new AddUserWidget(this);
        connect(auw->goBack, &QPushButton::clicked, this,
                [this] () {delete auw; setAdminPlusWindow();});
        connect(auw->addUser, &QPushButton::clicked, this,
                [this] () {
            slotSendToServer(
            "{cmd='add user';"
            "login='" + auw->getLogin() + "';"
            "pass='" + auw->getPassword() + "';"
            "name='" + auw->getName() + "';"
            "surname='" + auw->getSurname() + "';"
            "role='" + auw->getRole() + "';"
            "}");
        });

        setCentralWidget(auw);});

    connect(adminPlusW->goBack, &QPushButton::clicked, this, [this] {delete adminPlusW; setAuthorizationWindow();});


    setCentralWidget(adminPlusW);
}

void MyClient::setAdminWindow()
{
    adminW = new AdminWidget(this);

    connect(adminW->results, &QPushButton::clicked, this,
            [this] () {slotSendToServer("{cmd='view all results';}");
    });
    connect(adminW->groups, &QPushButton::clicked, this,
            [this] () {slotSendToServer("{cmd='view all groups';}");});
    connect(adminW->tests, &QPushButton::clicked, this,
            [this] () {slotSendToServer("{cmd='view all planned tests';}");});

    connect(adminW->goBack, &QPushButton::clicked, this,
            [this] () {delete adminW; setAuthorizationWindow();});

    setCentralWidget(adminW);
}

void MyClient::setTeacherWindow()
{
    teacherW = new TeacherWidget(this);
    connect(teacherW->newTaskButton, &QPushButton::clicked, this,
            [this] {
        delete teacherW;
        addTaskW = new AddTaskWidget(this);
        connect(addTaskW->addTaskQuit, &QPushButton::clicked, this, [this] {delete addTaskW; setTeacherWindow();});
        connect(addTaskW->addTaskSave, &QPushButton::clicked, this, [this]
        {   QString task = addTaskW->getTask();
            QString answer = addTaskW->getAnswer();
            QList <QString> answerOptions = addTaskW->getAnswerOptions();
            QString theme = addTaskW->getTheme();
            QString subject = addTaskW->getSubject();

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
                slotSendToServer(msg);
            }});
        setCentralWidget(addTaskW);});

    connect(teacherW->newTestButton, &QPushButton::clicked, this,
            [this] {delete teacherW; addTestW = new AddTestWidget(this); setCentralWidget(addTestW);});

    connect(teacherW->viewGroupsButton, &QPushButton::clicked, this,
            [this] {delete teacherW; addTestW = new AddTestWidget(this); setCentralWidget(addTestW);});

    connect(teacherW->viewResultsButton, &QPushButton::clicked, this,
            [this] {delete teacherW; addTestW = new AddTestWidget(this); setCentralWidget(addTestW);});

    connect(teacherW->goBack, &QPushButton::clicked, this, [this] {delete teacherW; setAuthorizationWindow();});
    setCentralWidget(teacherW);
}

