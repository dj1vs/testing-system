#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QtGlobal>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QTcpServer>
#include <QMessageBox>
#include <QDebug>
#include <QErrorMessage>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QComboBox>
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

class MyClient : public QMainWindow
{
    Q_OBJECT

public:
    MyClient(const QString& strHost, int nPort, QWidget *parent = nullptr);
    ~MyClient();
private:
    QErrorMessage *d;

    QLabel* authorizeLoginLabel;
    QLabel* authorizePasswordLabel;
    QLineEdit* authorizeLogin;
    QLineEdit* authorizePassword;
    QPushButton* authorize;
    QVBoxLayout *authorizeLayout;

    QPushButton *addStudentsGroup;
    QPushButton *addStudentsToGroup;
    QPushButton *appointGroup;
    QPushButton *addUser;
    QVBoxLayout *adminPlusLayout;

    QLabel *addGroupTitleLabel;
    QLineEdit *addGroupTitle;
    QPushButton *sendGroup;
    QPushButton *addGroupGoBack;
    QVBoxLayout *addGroupLayout;

    QLabel *addToGroupNameLabel;
    QLabel *addToGroupSurnameLabel;
    QLabel *addToGroupTitleLabel;
    QLineEdit *addToGroupName;
    QLineEdit *addToGroupSurname;
    QLineEdit *addToGroupTitle;
    QPushButton *sendToGroup;
    QPushButton *addToGroupGoBack;
    QVBoxLayout *addToGroupLayout;

    QLabel *appointGroupTeacherNameLabel;
    QLabel *appointGroupTeacherSurnameLabel;
    QLabel *appointGroupTitleLabel;
    QLineEdit *appointGroupTeacherName;
    QLineEdit *appointGroupTeacherSurname;
    QLineEdit *appointGroupTitle;
    QPushButton *sendAppointGroup;
    QPushButton *appointGroupGoBack;
    QVBoxLayout *appointGroupLayout;

    QLabel *addUserLoginLabel;
    QLabel *addUserPasswordLabel;
    QLabel *addUserNameLabel;
    QLabel *addUserSurnameLabel;
    QLabel *addUserRoleLabel;
    QComboBox *addUserBox;
    QLineEdit *addUserLogin;
    QLineEdit *addUserPassword;
    QLineEdit *addUserName;
    QLineEdit *addUserSurname;
    QPushButton *addUserButton;
    QPushButton *addUserGoBack;
    QFormLayout *addUserLayout;

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
    QList <QList <QString>> taskList;
    QPushButton *addSelectedTaskButton;
    QTableView *pickedTasksTableView;
    QStandardItemModel *pickedTasksModel;
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
    void showError(QString err);
    void showMsg(QString msg);
    QString cutArg(QString str, QString cmd);
private slots:
    void slotReadyRead          (                            );
    void slotError              (QAbstractSocket::SocketError);
    void slotSendToServer       (QString msg                 );

    void setAuthorizationWindow();
    void hideAuthorizationWindow();

    void setAdminPlusWindow();
    void hideAdminPlusWindow();

    void setAdminWindow();
    void hideAdminWindow();

    void setTeacherWindow();
    void hideTeacherWindow();



    void setAddGroupWindow();
    void hideAddGroupWindow();

    void setAddToGroupWindow();
    void hideAddToGroupWindow();

    void setAppointGroupWindow();
    void hideAppointGroupWindow();

    void setAddUserWindow();
    void hideAddUserWindow();

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

    void logInToSystem ();
    void sendGroupToSystem();
    void sendToGroupToSystem();
    void sendAppointGroupToSystem();
    void sendUserToSystem();
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
};
#endif // MYCLIENT_H
