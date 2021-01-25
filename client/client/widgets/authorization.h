#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Authorization : public QWidget
{
    Q_OBJECT
public:
    explicit Authorization(QWidget *parent = nullptr);
private:
    QLabel* loginLabel;
    QLabel* passwordLabel;
    QLineEdit* login;
    QLineEdit* password;
    QPushButton* authorize;
    QVBoxLayout *layout;
signals:
    void logInButtonClicked();

};

#endif // AUTHORIZATION_H
