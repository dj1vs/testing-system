// Copyright 2021 Dmitriy Trifonov
#ifndef ADDUSERWIDGET_H
#define ADDUSERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

class AddUserWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AddUserWidget(QWidget *parent = nullptr);
    QString getLogin() const {return login->text(); }
    QString getPassword() const {return password->text(); }
    QString getName() const {return name->text(); }
    QString getSurname() const {return surname->text(); }
    QString getRole() const {return roleBox->itemText(roleBox->currentIndex()); }

 public:
    QPushButton *addUser;
    QPushButton *goBack;

 private:
    QLabel *loginLabel;
    QLabel *passwordLabel;
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *roleLabel;
    QComboBox *roleBox;
    QLineEdit *login;
    QLineEdit *password;
    QLineEdit *name;
    QLineEdit *surname;
};

#endif  // ADDUSERWIDGET_H
