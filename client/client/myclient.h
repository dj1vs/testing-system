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
    QPushButton *adminViewGrades;
    QPushButton *adminViewTasks;
    QVBoxLayout *adminLayout;

    QTableView *allResultsTable;
    QStandardItemModel *allResultsModel;
    QPushButton *allResultsGoBack;
    QVBoxLayout *viewAllResultsLayout;
    QList <QList<QString>> allResultsList;

    QTableView *allGroupsTable;
    QStandardItemModel *allGroupsModel;
    QPushButton *allGroupsGoBack;
    QVBoxLayout *viewAllGroupsLayout;
    QList <QString> allGroupsList;
    QList <QList<QString>> groupStudents;
    QList <QList <QString>> groupTeachers;

    QTcpSocket* m_pTcpSocket;
    quint16 m_nNextBlockSize;
private:
    const int WINH = 350;
    const int WINW = 700;
private:
    void showError(QString err);
    void showMsg(QString msg);
    QString cutArg(QString str, QString cmd);
private slots:
    void slotReadyRead          (                            );
    void slotError              (QAbstractSocket::SocketError);
    void slotSendToServer       (QString msg                 );
    void slotConnected          (                            );

    void setAuthorizationWindow();
    void hideAuthorizationWindow();

    void setAdminPlusWindow();
    void hideAdminPlusWindow();

    void setAdminWindow();
    void hideAdminWindow();


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

    void solveMsg(QString msg);

    void logInToSystem ();
    void sendGroupToSystem();
    void sendToGroupToSystem();
    void sendAppointGroupToSystem();
    void sendUserToSystem();

    void showGroupTeachers();
    void showGroupStudents();
    void askGroupTeachers(QString groupName);
    void askGroupStudents(QString groupName);
signals:
    void allResultsCollected();
    void allGroupsCollected();
    void groupTeachersCollected();
    void groupStudentsCollected();
};
#endif // MYCLIENT_H
