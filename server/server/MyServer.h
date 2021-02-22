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

#include <utility>  //  for std::swap

#include "../../lib/DateConverter.h"
#include "../../lib/StringOperator.h"

class MyServer : public QObject {
    Q_OBJECT

 public:
    explicit MyServer(int nPort, QObject *parent = nullptr);

 private:
    //  оснавная переменная для работы с сервером
    QTcpServer* m_ptcpServer;
    //  хранение длины следующего полученного от сокета блока
    quint16 m_nNextBlockSize;

    //  переменная БД
    QSqlDatabase db;

 private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);

    //  обработать полученное от пользователя сообщение и получить
    //  нужные для отправки данные
    void solveMsg(QTcpSocket* pSocket, QString msg);

    //  вывести в консоль ошибку, возникшую при выполнении SQL запроса
    void printSQLError(QSqlQuery query) {qDebug() << "Can not run database query :("
                                         << query.lastError().databaseText()
                                         << query.lastError().driverText(); }
 public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

#endif  // MYSERVER_H
