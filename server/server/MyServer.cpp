#include "MyServer.h"

MyServer::MyServer(int nPort, QObject *parent) : QObject(parent),
                                                m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
            qDebug() << "Server Error";
            qDebug() << "Unable to start the server:";
            qDebug() << m_ptcpServer->errorString();
            m_ptcpServer->close();
            return;
        }
    connect(m_ptcpServer, SIGNAL(newConnection()),
                this,         SLOT(slotNewConnection())
               );
    qDebug() << "Server started succesfully :)\n";

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("systemdb");
    db.setUserName("postgres");
    db.setPassword("123");
    if(db.open())
        qDebug() << "Succesfully connected to database! :)";
    else
        qDebug() << "Can not connect to DB :(\n";
}

void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    qDebug() << "New client connected!";
    qDebug() << "Adress: " + pClientSocket->peerAddress().toString();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );
    sendToClient(pClientSocket, "{cmd='welcome';}");
}

void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
        QDataStream in(pClientSocket);
        in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (pClientSocket->bytesAvailable() < static_cast<qint64>(sizeof(quint16))) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QTime   time;
            QString str;
            in >> time >> str;

            QString strMessage = str;
            solveMsg(pClientSocket, strMessage);

            m_nNextBlockSize = 0;
        }
}

void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

void MyServer::solveMsg(QTcpSocket* pSocket, QString msg)
{
    if(msg[0] != '{')
    {
        qDebug() << "Invalid package: " + msg;
        return;
    }
    QString cmd = cutArg(msg, "cmd");
    if(cmd == "authorize")
    {
        QString login = cutArg(msg, "login");
        QString pass = cutArg(msg, "pass");
        QString sqlRequest = "SELECT role, id FROM users WHERE ";
        sqlRequest += "login = '" + login + "' AND ";
        sqlRequest += "password = '" + pass + "'";
        QSqlQuery query = QSqlQuery(db);
        if (!query.exec(sqlRequest))
        {
            qDebug() << "Can not run database query :(";
            qDebug() << query.lastError().databaseText();
            qDebug() << query.lastError().driverText();
        }
        else
        {
            if(query.size() == 0)
                sendToClient(pSocket, "{cmd='authorize';status='1';}");
            else
            {
                while(query.next())
                {
                    QString str = "{cmd='authorize';status='0';role='";
                    str += query.record().field(0).value().toString() + "';";
                    str += "id='" + query.record().field(1).value().toString()+"';}";
                    sendToClient(pSocket, str);
                }
            }
        }
    }
    else if(cmd == "add group")
    {
        QString groupName = cutArg(msg, "groupname");
        QSqlQuery query = QSqlQuery(db);
        query.prepare("INSERT INTO groups (name) "
                       "VALUES (:name)");
        query.bindValue(":name", groupName);
        if(query.exec())
            sendToClient(pSocket,  "{cmd='add group';status='0';}");
        else
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
    }
    else if (cmd == "add to group")
    {
        int groupId = 0;
        int userId = 0;
        QString name = cutArg(msg, "studentsname");
        QString surname = cutArg(msg, "studentssurname");
        QString title = cutArg(msg, "groupname");
        QString sqlRequest = "SELECT id FROM groups WHERE name = '" + title + "';";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(sqlRequest))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            if(!query.size())
            {
                sendToClient(pSocket, "{cmd='add to group';status='1';}");
                return;
            }
            else
                while(query.next())
                    groupId = query.record().field(0).value().toInt();
        }
        qDebug() << groupId;
        sqlRequest = "SELECT id FROM users WHERE name = '" + name;
        sqlRequest += "' AND surname = '" + surname + "';";
        if(!query.exec(sqlRequest))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            if(!query.size())
            {
                sendToClient(pSocket, "{cmd='add to group';status='2';}");
                return;
            }
            else
                while(query.next())
                    userId = query.record().field(0).value().toInt();
        }
        sqlRequest = "SELECT group_by_user.userId FROM group_by_user";
        sqlRequest += " INNER JOIN users ON users.id = group_by_user.userId";
        sqlRequest += " INNER JOIN groups ON groups.id = group_by_user.groupId";
        sqlRequest += " WHERE users.id = " + QString::number(userId);
        sqlRequest += "AND  groups.id = " + QString::number(groupId) + ';';
        if(!query.exec(sqlRequest))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else if(query.size())
        {
            sendToClient(pSocket, "{cmd='add to group';status='3';}");
            return;
        }

        query.prepare("INSERT INTO group_by_user (userId, groupId) "
                      "VALUES (:userId, :groupId)");
        query.bindValue(":userId", userId);
        query.bindValue(":groupId", groupId);
        if(query.exec())
            sendToClient(pSocket,  "{cmd='add to group';status='0';}");
        else
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
    }
    else if(cmd == "appoint")
    {
        QString teacherName = cutArg(msg, "teachername");
        QString teacherSurname = cutArg(msg, "teachersurname");
        QString groupName = cutArg(msg, "groupname");
        int teacherId = 0;

        QString sqlRequest = "SELECT id FROM users WHERE ";
        sqlRequest += "role = 'teacher' AND name = '" + teacherName;
        sqlRequest += "' AND surname = '" + teacherSurname + "';";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(sqlRequest))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else if(!query.size())
        {
            sendToClient(pSocket, "{cmd='appoint';status='1';}");
            return;
        }
        else
            while(query.next())
                teacherId = query.record().field(0).value().toInt();
        sqlRequest = "SELECT id FROM groups WHERE name = '" + groupName + "';";
        if(!query.exec(sqlRequest))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else if(!query.size())
        {
            sendToClient(pSocket, "{cmd='appoint';status='2';}");
            return;
        }

        query.prepare("UPDATE groups SET teacherid = (:teacherid) "
                        "WHERE name = '" + groupName + "';");
        query.bindValue(":teacherid", teacherId);
        if(query.exec())
            sendToClient(pSocket, "{cmd='appoint';status='0';}");
        else
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
    }
    else if(cmd == "add user")
    {
        QString name = cutArg(msg, "name");
        QString surname = cutArg(msg, "surname");
        QString login = cutArg(msg, "login");
        QString pass = cutArg(msg, "pass");
        QString role = cutArg(msg, "role");

        QString sqlRequest = "SELECT id FROM users WHERE ";
        sqlRequest += "name = '" + name + "' AND ";
        sqlRequest += "surname = '" + surname + "' AND ";
        sqlRequest += "login = '" + login + "' AND ";
        sqlRequest += "password = '" + pass + "' AND ";
        sqlRequest += "role = '" + role + "';";
        qDebug() << sqlRequest;
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(sqlRequest))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else if (query.size())
        {
            sendToClient(pSocket, "{cmd='add user', status='1'");
            return;
        }
        qDebug() << "Hi!";
        query.prepare("INSERT INTO users (login, password, name, surname, role)"
                        "VALUES (:login, :password, :name, :surname, :role);");
        query.bindValue(":login", login);
        query.bindValue(":password", pass);
        query.bindValue(":name", name);
        query.bindValue(":surname", surname);
        query.bindValue(":role", role);

        if(!query.exec())
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
            sendToClient(pSocket, "{cmd='add user', status='0'");
    }
    else if(cmd == "view all results")
    {
        sendToClient(pSocket, "{cmd='view all results';status='started';}");
        QString req = "SELECT tests.name, tests.subject, users.name, users.surname, results.percent ";
        req += "FROM results INNER JOIN users ON results.studentid = users.id ";
        req += "INNER JOIN tests ON results.testid = tests.id;";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            if(!query.size())
                return;
            QList <QString> params = {"testname", "subject", "studentsname", "studentssurname", "percent"};
            while(query.next())
            {
                QString processMsg = "{cmd='view all results';";
                for(int i = 0; i < 5; ++i)
                    processMsg += params[i] + "='" + query.record().field(i).value().toString() + "';";
                processMsg += '}';
                sendToClient(pSocket, processMsg);
            }
            sendToClient(pSocket, "{cmd='view all results';status='sended';}");
        }
    }
    else if(cmd == "view all groups")
    {
        QString req = "SELECT name FROM groups";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            while(query.next())
            {
                QString processMsg = "{cmd='view all groups';name='" + query.record().field(0).value().toString()+ "';}";
                sendToClient(pSocket, processMsg);
            }
            sendToClient(pSocket, "{cmd='view all groups';status='sended';}");
        }
    }
    else if (cmd == "view group teachers")
    {
        QString req = "SELECT users.name, users.surname FROM groups ";
        req += "INNER JOIN users ON groups.teacherid = users.id WHERE groups.name = '" + cutArg(msg, "groupname") + "';";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            while(query.next())
            {
                QString procMsg = "{cmd='view group teachers';name='" + query.record().field(0).value().toString();
                procMsg += "';surname='" + query.record().field(1).value().toString() + "';}";
                sendToClient(pSocket, procMsg);
            }
            sendToClient(pSocket, "{cmd='view group teachers';status='sended';}");
        }
    }
    else if (cmd == "view group students")
    {
        QString req = "SELECT users.name, users.surname FROM group_by_user ";
        req += "INNER JOIN users ON group_by_user.userid = users.id ";
        req += "INNER JOIN groups ON group_by_user.groupid = groups.id ";
        req += "WHERE groups.name = '" + cutArg(msg, "groupname") + "';";
        QSqlQuery query = QSqlQuery(db);
        if (!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            while(query.next())
            {
                QString procMsg = "{cmd='view group students';name='" + query.record().field(0).value().toString();
                procMsg += "';surname='" + query.record().field(1).value().toString() + "';}";
                sendToClient(pSocket, procMsg);
            }
            sendToClient(pSocket, "{cmd='view group students';status='sended';}");
        }
    }
    else if(cmd == "view all planned tests")
    {
        QString req = "SELECT users.name, users.surname, tests.name, tests.subject, tests.planneddate ";
        req += "FROM tests INNER JOIN users ON tests.teacherid = users.id;";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            while(query.next())
            {
                QString procMsg = "{cmd='view all planned tests';";
                QList <QString> params = {"teachername", "teachersurname", "testname", "subject", "date"};
                for(int i = 0; i < query.record().count(); ++i)
                    procMsg += params[i] + "='" + query.record().field(i).value().toString() + "';";
                procMsg += '}';
                sendToClient(pSocket, procMsg);
            }
            sendToClient(pSocket, "{cmd='view all planned tests';status='sended';}");
        }
    }
    else if (cmd == "view test tasks")
    {
        sendToClient(pSocket, "{cmd='view test tasks';status='started';}");
        QString req = "SELECT tests.name, tasks.task, tasks.answeroptions, tasks.answer, tasks.theme ";
        req += "FROM test_by_task INNER JOIN tests ON tests.id = test_by_task.testid ";
        req += "INNER JOIN tasks ON tasks.id = test_by_task.taskid ";
        req += "WHERE tests.name = '" + cutArg(msg, "testname") + "';";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            while(query.next())
            {
                qDebug() << query.record();
                QString str = "{cmd='view test tasks';";
                QList <QString> params = {"testname", "taskname", "answeroptions","answertext","theme"};
                for(int i = 0; i < query.record().count(); ++i)
                    str += params[i] + "='" + query.record().field(i).value().toString() + "';";
                str + "}";
                sendToClient(pSocket, str);
            }
            sendToClient(pSocket, "{cmd='view test tasks';status='sended';}");
        }
    }
    else if (cmd == "add task")
    {
        quint16 id = cutArg(msg, "id").toInt();
        QString tasktext = cutArg(msg, "tasktext");
        QString answer = cutArg(msg, "answer");
        QString theme = cutArg(msg, "theme");
        QString subject = cutArg(msg, "subject");
        QString answerOptions = cutArg(msg, "answerOptions");

        QSqlQuery query = QSqlQuery(db);
        query.prepare("INSERT INTO tasks (task, answeroptions, answer, theme, subject, teacherid) "\
                        "VALUES (:task, :answeroptions, :answer, :theme, :subject, :teacherid);");
        query.bindValue(":task", tasktext);
        query.bindValue(":answeroptions", answerOptions);
        query.bindValue(":answer", answer);
        query.bindValue(":theme", theme);
        query.bindValue(":subject", subject);
        query.bindValue(":teacherid", id);
        qDebug() << query.lastQuery();
        if(!query.exec())
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else
        {
            qDebug() << "Wow!";
            sendToClient(pSocket, "{cmd='add task';status='sended';}");
        }
    }
    else if (cmd == "validate tasks amount")
    {
//        QString msg = "{cmd='validate tasks amount';";
//        msg += "theme='"  + addTestRandomTheme->text() + "';";
//        msg += "subject='" + addTestRandomSubject->text() + "';";
//        msg += "testsize='" + QString::number(addTestRandomAmount->value()) + "';";
//        msg += "author='" + QString((addTestRandomMine->isChecked() ? "ME" : "ALL")) + "';";
//        msg += '}';
        QString theme = cutArg(msg, "theme");
        QString subject = cutArg(msg, "subject");
        QString amount = cutArg(msg, "testsize");
        QString author = cutArg(msg, "author");
        QString teacherId = cutArg(msg, "teacherid");

        QString req = "SELECT id FROM tasks WHERE theme = '" + theme + "' AND ";
        req += "subject = '" + subject + "'";
        if(author == "ME")
                req += " AND teacherid=" + teacherId;
        req += " LIMIT " + amount + ";";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
        else {
            if(query.size() < amount.toInt())
            {
                sendToClient(pSocket, "{cmd='validate tasks amount';status='not enough';tasksamount='" + QString::number(query.size()) + "';}");
                qDebug() << ":(";
            }
            else
            {
                sendToClient(pSocket, "{cmd='validate tasks amount';status='success';}");
                qDebug() << ":)";
            }
        }


    }
    else if (cmd == "add test")
    {
//        QString msg = "{cmd='add test';"
//                        "amount='" + QString::number(addTestRandomAmount->value()) + "';";
//                        "teacherid='" + QString::number(id) + "';"
//                        "theme='" + addTestRandomTheme->text() + "';"
//                        "subject='" + addTestRandomSubject->text() + "';"
//                        "testname='" + addTestRandomName->text() + "';"
//                        "date='" + date + "';"
//                        "author='" + (addTestRandomMine->isChecked() ? "ME" : "ALL") + "';}";
        QString amount = cutArg(msg, "amount");
        QString teacherId = cutArg(msg, "teacherid");
        qDebug() << msg;
        QString theme = cutArg(msg, "theme");
        QString subject = cutArg(msg, "subject");
        QString testname = cutArg(msg, "testname");
        QString date = cutArg(msg, "date");
        QString author = cutArg(msg, "author");

        QList <quint32> id;
        QList <quint32> finish;

        quint32 testId;


        QSqlQuery query = QSqlQuery(db);
        query.prepare("INSERT INTO tests (teacherid, name, subject, planneddate)"
                        "VALUES (:teacherid, :name, :subject, :planneddate)");
        query.bindValue(":teacherid", teacherId.toInt());
        query.bindValue(":name", testname);
        query.bindValue(":subject", subject);
        query.bindValue(":planneddate", date);

        if(!query.exec())
        {
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
            return;
        }
        QString req = "SELECT id FROM tasks WHERE";
        if(author == "ME")
            req += " teacherid = " + teacherId + " AND";
        req += " theme = '" + theme + "' AND subject = '" + subject + "';";
        if(!query.exec(req))
        {
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
            return;
        }
        else
        {
            if(query.size() < amount.toInt())
            {
                qDebug() << query.size();
                return;
            }
            while(query.next())
            {
                id.push_back(query.record().field(0).value().toInt());
            }
        }

        if(id.size() == amount.toInt())
            finish = id;
        else
        {
            for(int i = 0; i < amount.toInt(); ++i)
            {
                  quint32 value = QRandomGenerator::global()->generate() % (id.size());
                  finish.push_back(value);
                  id.removeAt(value);
            }
        }

        if(!query.exec("SELECT COUNT(*) FROM tests"))
        {
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
            return;
        }
        else
        {
            while(query.next())
                testId = query.record().field(0).value().toInt();
        }

        for(int i = 0; i < finish.size(); ++i)
        {
            query.prepare("INSERT INTO test_by_task (taskid, testid)"
                          "VALUES (:taskid, :testid)");
            query.bindValue(":testid", testId);
            qDebug() << finish[i];
            query.bindValue(":taskid", finish[i]);

            if(!query.exec())
            {
                qDebug() << "Can not run database query :("
                << query.lastError().databaseText()
                << query.lastError().driverText();
                return;
            }
        }
        qDebug() << ":)";

        sendToClient(pSocket, "{cmd='add test';status='yes';}");
    }
    else if(cmd == "get tasks")
    {
        QString req = "SELECT * FROM tasks";
        QSqlQuery query = QSqlQuery(db);
        if(!query.exec(req))
        {
            qDebug() << "Can not run database query :("
            << query.lastError().databaseText()
            << query.lastError().driverText();
            return;
        }
        while(query.next())
        {
            QList <QString> args = {"taskid", "subject", "tasktext", "answeroptions", "answertext", "theme", "teacherid"};
            QString msg = "{cmd='get tasks';status='sending';";
            for(int i = 0; i < 7; ++i)
            {
                msg+= args[i] + "='" + query.record().field(i).value().toString() + "';";
            }
            msg += "}";
            qDebug() << msg;
            sendToClient(pSocket, msg);
        }
        sendToClient(pSocket, "{cmd='get tasks';status='sended';}");

    }
}

QString MyServer::cutArg(QString str, QString cmd)
{
    int pos1 = str.indexOf(cmd) + cmd.size() + 2;
    int pos2 = str.midRef(pos1, str.size()).indexOf("'");
    return str.mid(pos1, pos2);
}
