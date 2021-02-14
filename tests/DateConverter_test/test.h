// Copyright 2021 Dmitriy Trifonov
#ifndef TEST_H
#define TEST_H

#include <QObject>

class test : public QObject {
    Q_OBJECT

 public:
    explicit test(QObject *parent = nullptr)  : QObject(parent) {}

 private slots:
    void DateToStringFormat();
};

#endif  // TEST_H
