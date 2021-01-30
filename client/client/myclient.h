#ifndef MYCLIENT_H
#define MYCLIENT_H

#include "lib/DateConverter.h"

#include "widgets/AuthorizationWidget.h"

#include "widgets/AdminPlus/AdminPlusWidget.h"
#include "widgets/AdminPlus/AddGroupWidget.h"
#include "widgets/AdminPlus/AddToGroupWidget.h"
#include "widgets/AdminPlus/AppointGroupWidget.h"
#include "widgets/AdminPlus/AddUserWidget.h"

#include "widgets/Admin/AdminWidget.h"
#include "widgets/Admin/AllResultsWidget.h"
#include "widgets/Admin/AllGroupsWidget.h"
#include "widgets/Admin/AllTestsWidget.h"

#include "widgets/Teacher/AddTaskWidget.h"
#include "widgets/Teacher/TeacherWidget.h"
#include "widgets/Teacher/AddTestWidget.h"
#include "widgets/Teacher/TeacherGroupsWidget.h"
#include "widgets/Teacher/TeacherResultsWidget.h"

#include <QtGlobal>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QTcpServer>
#include <QMessageBox>
#include <QDebug>
#include <QSlider>
#include <QStatusBar>
#include <QSignalMapper>
#include <QCheckBox>
#include <QTextBrowser>
#include <QTextEdit>
#include <QInputDialog>
#include <QSpinBox>
#include <QItemSelectionModel>

class MyClient : public QMainWindow
{
    Q_OBJECT

public:
    MyClient(const QString& strHost, int nPort, QWidget *parent = nullptr);
    ~MyClient();
private:
    AuthorizationWidget *aw;

    AdminPlusWidget *adminPlusW;
    AddGroupWidget *agw;
    AddToGroupWidget *atgw;
    AppointGroupWidget *appgw;
    AddUserWidget *auw;

    AdminWidget *adminW;
    AllResultsWidget *arw;
    QList <QList <QString>> allResultsList;
    AllGroupsWidget *allGroupsW;
    QList <QString> allGroupsList;
    QList <QList<QString>> groupStudents;
    QList <QList <QString>> groupTeachers;
    AllTestsWidget *atw;
    QList <QList<QString>> allPlannedTestsTaskList;
    QList <QList <QString>> allPlannedTestsList;

    TeacherWidget *teacherW;
    AddTaskWidget *addTaskW;
    AddTestWidget *addTestW;
    QList <QList <QString>> allTasksList;
    int testid = 0;
    TeacherGroupsWidget *teacherGroupsW;
    TeacherResultsWidget *teacherResultsW;

    QTcpSocket* m_pTcpSocket;
    quint16 m_nNextBlockSize;
private:
    const int WINH = 450;
    const int WINW = 700;
    quint16 id = -1;
private:
    void showError(QString err) {QErrorMessage *d = new QErrorMessage(this); d->showMessage(err);};
    void showMsg(QString msg){statusBar()->showMessage(msg);};
    QString cutArg(QString str, QString cmd);
private slots:
    void slotReadyRead          (                            );
    void slotError              (QAbstractSocket::SocketError);
    void slotSendToServer       (QString msg                 );

    void setAuthorizationWindow();
    void setAdminPlusWindow();
    void setAdminWindow();
    void setTeacherWindow();

    void solveMsg(QString msg);
};
#endif // MYCLIENT_H
