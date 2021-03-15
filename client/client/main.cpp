// Copyright 2021 Dmitriy Trifonov

#include <QApplication>
#include "./myclient.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MyClient w("localhost", 2323);
    w.setStyleSheet("color: white; background-color: rgb(46, 52, 54)");
    w.show();
    return a.exec();
}
