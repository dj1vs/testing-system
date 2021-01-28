#ifndef MYCLIENT_H
#define MYCLIENT_H

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

#include <QtGlobal>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QTcpServer>
#include <QMessageBox>
#include <QDebug>
#include <QErrorMessage>
#include <QListView>
#include <QSlider>
#include <QStatusBar>
#include <QSignalMapper>
#include <QDateEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextBrowser>
#include <QListView>
#include <QStringListModel>
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


    QPushButton *newTaskButton;
    QPushButton *newTestButton;
    QPushButton *viewTeacherGroupsButton;
    QPushButton *viewTeacherResultsButton;
    QVBoxLayout *teacherWindowLayout;
    ///////////////////////////////////
    QLabel *addTaskQuesitionLabel;
    QLabel *addTaskAnswerLabel;
    QLabel *addTaskAnswerOptionsLabel;
    QLabel *addTaskThemeLabel;
    QLabel *addTaskSubjectLabel;
    QPushButton *addTaskSave;
    QPushButton *addTaskQuit;
    QPushButton *addTaskNewOption;
    QPushButton *addTaskDeleteEmpty;
    QTextEdit *addTaskQuesition;
    QTextEdit *addTaskAnswer;
    QStringListModel *addTaskAnswerOptionsModel;
    QListView *addTaskAnswerOptionsView;
    QLineEdit *addTaskTheme;
    QLineEdit *addTaskSubject;
    QVBoxLayout *addTaskLayout;
    QList <QString> addTaskAnswerOptions;


    QPushButton *addTestQuit;
    QPushButton *addTestGoRandom;
    QPushButton *addTestGoManual;
    QVBoxLayout *addTestLayout;

    QLabel *addTestRandomThemeLabel;
    QLabel *addTestRandomSubjectLabel;
    QLabel *addTestRandomAuthorLabel;
    QLabel *addTestRandomAmountLabel;
    QLabel *addTestRandomNameLabel;
    QLabel *addTestRandomDateLabel;
    QDateEdit *addTestRandomDate;
    QLineEdit *addTestRandomTheme;
    QLineEdit *addTestRandomSubject;
    QLineEdit *addTestRandomName;
    QRadioButton *addTestRandomMine;
    QRadioButton *addTestRandomAll;
    QSpinBox *addTestRandomAmount;
    QPushButton *addTestRandomQuit;
    QPushButton *addTestRandomSave;
    QVBoxLayout *addTestRandomLayout;

    QTableView *allTasksTableView;
    QStandardItemModel *allTasksModel;
    QItemSelectionModel *allTasksSelect;
    QList <QList <QString>> taskList;
    QList <QList <QString>> pickedTasksList;
    QPushButton *addSelectedTaskButton;
    QTableView *pickedTasksTableView;
    QStandardItemModel *pickedTasksModel;
    QItemSelectionModel *pickedTasksSelect;
    QPushButton *addTestManualDeleteSelected;
    QPushButton *sendManualTest;
    QPushButton *addTestManualQuit;
    QVBoxLayout *addTestManualLayout;





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
    void hideTeacherWindow();
///////////////////////////////////
    void setViewAllGroupsWindow();

    void setAddTaskWindow();
    void hideAddTaskWindow();

    void setAddTestWindow();
    void hideAddTestWindow();

    void setAddTestRandomWindow();
    void hideAddTestRandomWindow();
    void setAddTestManualWindow();
    void hideAddTestManualWindow();
    ///////////////////////

    void solveMsg(QString msg);

    ////////////////////
    void sendTaskToSystem();

    void showAddAnswerOptions();
    void addTaskAnswerOptionsDeleteEmpty();
};
#endif // MYCLIENT_H
