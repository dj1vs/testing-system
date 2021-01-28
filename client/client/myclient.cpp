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
            qDebug() << id;
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
            setViewAllGroupsWindow();
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
        if(t)
        {
            QDate d = addTestRandomDate->date();
            QString date = QString::number(d.year()) + (d.month() < 10 ? "-0" : "-") + QString::number(d.month()) + (d.day() < 10 ? "-0" : "-") + QString::number(d.day());
            QString msg = "{cmd='add test';"
                            "amount='" + QString::number(addTestRandomAmount->value()) + "';"
                            "teacherid='" + QString::number(id) + "';"
                            "theme='" + addTestRandomTheme->text() + "';"
                            "subject='" + addTestRandomSubject->text() + "';"
                            "testname='" + addTestRandomName->text() + "';"
                            "date='" + date + "';"
                            "author='" + (addTestRandomMine->isChecked() ? "ME" : "ALL") + "';}";
            qDebug() << msg;
            slotSendToServer(msg);
        }
    }
    else if(cmd == "add test")
        showMsg("added!");
    else if(cmd == "get tasks")
    {
        if(cutArg(msg,"status") == "sending")
            taskList.push_back({cutArg(msg, "taskid"), cutArg(msg, "subject"),
            cutArg(msg, "tasktext"), cutArg(msg, "answeroptions"),
            cutArg(msg, "answertext"), cutArg(msg, "theme"), cutArg(msg, "teacherid")});
        else
        {
            showMsg("Task list collected");
            setAddTestManualWindow();
        }
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

void MyClient::setViewAllGroupsWindow()
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
    connect(addTestGoManual, &QPushButton::clicked, this, [this] {hideAddTestWindow(); taskList.clear(); slotSendToServer("{cmd='get tasks';}");});
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
    allTasksTableView = new QTableView(this);
    allTasksModel = new QStandardItemModel(taskList.size(), taskList[0].size(), this);
    QList <QString> params = {"ID", "Subject", "Task", "Answer Options", "Answer", "Theme", "Teacher Id"};
    for(int i = 0; i < taskList[0].size(); ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        allTasksModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < taskList.size(); ++row)
    {
        for(int col = 0; col < taskList[0].size(); ++col)
        {
            QModelIndex index=allTasksModel->index(row,col,QModelIndex());
            allTasksModel->setData(index, taskList[row][col]);
        }
    }
    allTasksTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allTasksTableView->setModel(allTasksModel);
    allTasksSelect = allTasksTableView->selectionModel();

    addSelectedTaskButton = new QPushButton("add selected");
    connect(addSelectedTaskButton, &QPushButton::clicked, this, [this]
    {
        for(int i = 0; i < allTasksSelect->selectedRows().size(); ++i)
        {
            pickedTasksList.push_back(taskList[i]);
        }
        for(int i = pickedTasksModel->rowCount(); i < pickedTasksList.size(); ++i)
        {
            pickedTasksModel->insertRow(i);
        }
        for(int row = 0; row < pickedTasksList.size(); ++row)
        {
            for(int col = 0; col < pickedTasksList[0].size(); ++col)
            {
                QModelIndex index= pickedTasksModel->index(row,col,QModelIndex());
                pickedTasksModel->setData(index, pickedTasksList[row][col]);
            }
        }
        pickedTasksTableView->setModel(pickedTasksModel);
    });
    pickedTasksTableView = new QTableView(this);
    pickedTasksModel = new QStandardItemModel(0, taskList[0].size(), this);
    for(int i = 0; i < taskList[0].size(); ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        pickedTasksModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    pickedTasksTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pickedTasksTableView->setModel(pickedTasksModel);
    pickedTasksSelect = pickedTasksTableView->selectionModel();

    addTestManualDeleteSelected = new QPushButton("delete selected");

    connect(addTestManualDeleteSelected, &QPushButton::clicked, this, [this]
    {
        for(int i = 0; i < pickedTasksSelect->selectedRows().size(); ++i)
        {
            pickedTasksList.removeAt(i);
            pickedTasksModel->removeRow(i);
        }
        pickedTasksTableView->setModel(pickedTasksModel);
    });

    sendManualTest = new QPushButton("send");

    addTestManualQuit = new QPushButton("quit");
    connect(addTestManualQuit, &QPushButton::clicked, this, [this] {hideAddTestManualWindow(); setAddTestWindow();});

    addTestManualLayout = new QVBoxLayout();
    addTestManualLayout->addWidget(allTasksTableView);
    addTestManualLayout->addWidget(addSelectedTaskButton);
    addTestManualLayout->addWidget(pickedTasksTableView);
    addTestManualLayout->addWidget(addTestManualDeleteSelected);
    addTestManualLayout->addWidget(sendManualTest);
    addTestManualLayout->addWidget(addTestManualQuit);

    //setNewLayout(addTestManualLayout);
}
void MyClient::hideAddTestManualWindow()
{
    disconnect(addTestManualQuit);
    allTasksTableView->close();
    addSelectedTaskButton->close();
    pickedTasksTableView->close();
    addTestManualDeleteSelected->close();
    sendManualTest->close();
    addTestManualQuit->close();
}
