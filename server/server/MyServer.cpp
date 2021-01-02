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
        QString sqlRequest = "SELECT role FROM users WHERE ";
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
                    QSqlRecord record = query.record();
                    QSqlField field = record.field(0);
                    QString var = field.value().toString();
                    QString str = "{cmd='authorize';status='0';role='";
                    str += var + "'}";
                    sendToClient(pSocket, str);
                }
            }
        }
    }
    else if(cmd == "add group")
    {
        QString groupName = cutArg(msg, "groupname");
        QString sqlRequest;
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
}

QString MyServer::cutArg(QString str, QString cmd)
{
    int pos1 = str.indexOf(cmd) + cmd.size() + 2;
    int pos2 = str.midRef(pos1, str.size()).indexOf("'");
    return str.mid(pos1, pos2);
}
