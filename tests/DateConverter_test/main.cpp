// Copyright 2021 Dmitriy Trifonov
#include <QCoreApplication>
#include <QTest>
#include <QFile>
#include <iostream>
#include "test.h"

int main(int argc, char *argv[]) {
    freopen("testing.log", "w", stdout);
    QCoreApplication a(argc, argv);
    QTest::qExec(new test, argc, argv);
    QFile file("testing.log");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
    }
    return 0;
}
