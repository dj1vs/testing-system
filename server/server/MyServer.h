#ifndef MYSERVER_H
#define MYSERVER_H

#include "../../client/client/lib/DateConverter.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <iostream>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlField>
#include <QSqlTableModel>
#include <QRandomGenerator>

class QTcpServer;
class QTcpSocket;


class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(int nPort, QObject *parent = nullptr);
private:
    QTcpServer* m_ptcpServer;
    quint16     m_nNextBlockSize;
    QSqlDatabase db;
private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    void solveMsg(QTcpSocket* pSocket, QString msg);
    QString cutArg(QString str, QString cmd);
public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
};

#endif // MYSERVER_H
