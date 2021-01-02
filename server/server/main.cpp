#include <QCoreApplication>
#include "MyServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyServer server(2323);
    return a.exec();
}
