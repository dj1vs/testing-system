#ifndef MYCLIENT_H
#define MYCLIENT_H

#include "widgets/AuthorizationWidget.h"

#include "widgets/AdminPlus/AdminPlusWidget.h"
#include "widgets/AdminPlus/AddGroupWidget.h"
#include "widgets/AdminPlus/AddToGroupWidget.h"
#include "widgets/AdminPlus/AppointGroupWidget.h"
#include "widgets/AdminPlus/AddUserWidget.h"

#include "widgets/Admin/AdminWidget.h"

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
#include <QTableView>
#include <QAbstractTableModel>
#include <QStandardItemModel>
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

    QErrorMessage *d;

    QPushButton *adminViewResults;
    QPushButton *adminViewGroups;
    QPushButton *adminViewPlannedTest;
    QVBoxLayout *adminLayout;

    QTableView *allResultsTable;
    QStandardItemModel *allResultsModel;
    QPushButton *allResultsGoBack;
    QPushButton *sortAllResults;
    QVBoxLayout *viewAllResultsLayout;
    QList <QList<QString>> allResultsList;
    QLabel *allResultsSortNameLabel;
    QLabel *allResultsSortSurnameLabel;
    QLabel *allResultsSortSubjectLabel;
    QLabel *allResultsSortTestLabel;
    QLineEdit *allResultsSortName;
    QLineEdit *allResultsSortSurname;
    QLineEdit *allResultsSortSubject;
    QLineEdit *allResultsSortTest;
    QPushButton *allResultsSortSave;
    QVBoxLayout *allResultsSortLayout;

    QTableView *allGroupsTable;
    QStandardItemModel *allGroupsModel;
    QPushButton *allGroupsGoBack;
    QVBoxLayout *viewAllGroupsLayout;
    QList <QString> allGroupsList;
    QList <QList<QString>> groupStudents;
    QList <QList <QString>> groupTeachers;

    QTableView *allPlannedTestsTable;
    QStandardItemModel *allPlannedTestsModel;
    QPushButton *allPlannedTestsGoBack;
    QPushButton *allPlannedTestsSort;
    QVBoxLayout *allPlannedTestsLayout;
    ////////////////////////////////////////////
    QList <QList <QString>> allPlannedTestsList;
    QLabel *allPlannedTestsSortNameLabel;
    QLabel *allPlannedTestsSortSurnameLabel;
    QLabel *allPlannedTestsSortTestLabel;
    QLabel *allPlannedTestsSortDateLabel;
    QLabel *allPlannedTestsSortSubjectLabel;
    QLineEdit *allPlannedTestsSortName;
    QLineEdit *allPlannedTestsSortSurname;
    QLineEdit *allPlannedTestsSortTest;
    QDateEdit *allPlannedTestsSortDate;
    QLineEdit *allPlannedTestsSortSubject;
    QRadioButton *allPlannedTestsSortViewPast;
    QRadioButton *allPlannedTestsSortViewFuture;
    QRadioButton *allPlannedTestsSortViewAll;
    QPushButton *allPlannedTestsSortSave;
    QVBoxLayout *allPlannedTestsSortLayout;
    ///////////////////////////////////////
    QList <QList<QString>> allPlannedTestsTaskList;
    quint16 tasksAmount = 0;
    quint16 currTask = 0;
    QPushButton *allPlannedTestsTaskNext;
    QPushButton *allPlannedTestsTaskPrev;
    QTextBrowser *allPlannedTestsTaskText;
    QTextBrowser *allPlannedTestsTaskAnswer;
    QStringListModel *allPlannedTestsTaskAnswerOptionsModel;
    QListView *allPlannedTestsTaskAnswerOptionsView;
    QVBoxLayout *allPlannedTestsTaskLayout;

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
    void showError(QString err) {d->showMessage(err);};
    void showMsg(QString msg){statusBar()->showMessage(msg);};
    QString cutArg(QString str, QString cmd);
private slots:
    void slotReadyRead          (                            );
    void slotError              (QAbstractSocket::SocketError);
    void slotSendToServer       (QString msg                 );

    void setAuthorizationWindow();

    void setAdminPlusWindow();

    void setAdminWindow();
    void hideAdminWindow();

    void setTeacherWindow();
    void hideTeacherWindow();


//////////admin plus mode layouts////////////////
    void setAddGroupWindow();
    void setAddToGroupWindow();
    void setAppointGroupWindow();
    void setAddUserWindow();
//////////////////
    void setViewAllResultsWindow();
    void hideViewAllResultsWindow();

    void setViewAllGroupsWindow();
    void hideViewAllGroupsWindow();

    void setViewAllPlannedTestsWindow();
    void hideViewAllPlannedTestsWindow();

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

    ///////////////////

    void showAllResultsSort();
    void editAllResultsTable();

    void showGroupTeachers();
    void showGroupStudents();

    void showAllPlannedTestsSort();
    void editAllPlannedTestsTable();

    void showTestTasks();
    void showNextTask();
    void showPrevTask();

    void showAddAnswerOptions();
    void addTaskAnswerOptionsDeleteEmpty();

    void askGroupTeachers(QString groupName);
    void askGroupStudents(QString groupName);
signals:
    void allResultsCollected();
    void allGroupsCollected();
    void groupTeachersCollected();
    void groupStudentsCollected();
    void allPlannedTestsCollected();
    void testTasksCollected();
    void getTasks();
    void allTasksCollected();
};
#endif // MYCLIENT_H
