// Copyright 2021 Dmitriy Trifonov
#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlField>
#include <QSqlTableModel>
#include <QRandomGenerator>

#include <iostream>
#include <utility>

#include "../../lib/DateConverter.h"
#include "../../lib/StringOperator.h"

class QTcpServer;
class QTcpSocket;


class MyServer : public QObject {
    Q_OBJECT

 public:
    explicit MyServer(int nPort, QObject *parent = nullptr);

 private:
    QTcpServer* m_ptcpServer;
    quint16 m_nNextBlockSize;
    QSqlDatabase db;

 private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    void solveMsg(QTcpSocket* pSocket, QString msg);
    void printSQLError(QSqlQuery query) {qDebug() << "Can not run database query :("
                                         << query.lastError().databaseText()
                                         << query.lastError().driverText(); }



 public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

#endif  // MYSERVER_H
