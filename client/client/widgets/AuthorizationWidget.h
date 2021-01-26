#ifndef AUTHORIZATIONWIDGET_H
#define AUTHORIZATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AuthorizationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthorizationWidget(QWidget *parent = nullptr);
    QString getLogin() const {return login->text();};
    QString getPassword() const {return password->text();};

    QPushButton* authorize;
private:
    QLabel* loginLabel;
    QLabel* passwordLabel;
    QLineEdit* login;
    QLineEdit* password;
    QVBoxLayout *layout;
};

#endif // AUTHORIZATIONWIDGET_H
