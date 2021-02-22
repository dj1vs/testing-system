// Copyright 2021 Dmitriy Trifonov
#ifndef ADDTOGROUPWIDGET_H
#define ADDTOGROUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class AddToGroupWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AddToGroupWidget(QWidget *parent = nullptr);
    QString getName() const {return name->text(); }
    QString getSurame() const {return surname->text(); }
    QString getTitle() const {return title->text(); }

 public:
    QPushButton *sendToGroup;
    QPushButton *goBack;

 private:
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *titleLabel;
    QLineEdit *name;
    QLineEdit *surname;
    QLineEdit *title;
};

#endif  // ADDTOGROUPWIDGET_H
