#ifndef AUTHORIZATIONWIDGET_H
#define AUTHORIZATIONWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <KPasswordLineEdit>

class AuthorizationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthorizationWidget(QWidget *parent = nullptr);
    QString getLogin() const {return login->text();};
    QString getPassword() const {return password->password();};

    QPushButton* authorize;
private:
    QLineEdit* login;
    KPasswordLineEdit* password;
};

#endif // AUTHORIZATIONWIDGET_H
