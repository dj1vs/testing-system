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

    QLabel *addGroupTeacherNameLabel;
    QLabel *addGroupTeacherSurnameLabel;
    QLabel *addGroupTitleLabel;
    QLineEdit *addGroupTeacherName;
    QLineEdit *addGroupTeacherSurname;
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

    void solveMsg(QString msg);

    void logInToSystem ();
    void sendGroupToSystem();
    void sendToGroupToSystem();

    void showError(QString err);
    QString cutArg(QString str, QString cmd);
private slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(QString msg                 );
    void slotConnected   (                            );
};
#endif // MYCLIENT_H
