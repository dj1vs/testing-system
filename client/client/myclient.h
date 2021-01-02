#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QtGlobal>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTime>
#include <QTcpServer>
#include <QMessageBox>
#include <QDebug>
#include <QErrorMessage>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QComboBox>

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

    QTcpSocket* m_pTcpSocket;
    quint16 m_nNextBlockSize;
private:
    const int WINH = 200;
    const int WINW = 400;
private:
    void setAuthorizationWindow();
    void hideAuthorizationWindow();

    void setAdminPlusWindow();
    void hideAdminPlusWindow();

    void setAddGroupWindow();
    void hideAddGroupWindow();

    void setAddToGroupWindow();
    void hideAddToGroupWindow();

    void setAppointGroupWindow();
    void hideAppointGroupWindow();

    void setAddUserWindow();
    void hideAddUserWindow();

    void solveMsg(QString msg);

    void logInToSystem ();
    void sendGroupToSystem();
    void sendToGroupToSystem();
    void sendAppointGroupToSystem();
    void sendUserToSystem();

    void showError(QString err);
    QString cutArg(QString str, QString cmd);
private slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(QString msg                 );
    void slotConnected   (                            );
};
#endif // MYCLIENT_H
