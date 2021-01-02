#include "myclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyClient w("localhost", 2323);
    w.show();
    return a.exec();
}
