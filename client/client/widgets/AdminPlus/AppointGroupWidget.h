// Copyright 2021 Dmitriy Trifonov
#ifndef APPOINTGROUPWIDGET_H
#define APPOINTGROUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class AppointGroupWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AppointGroupWidget(QWidget *parent = nullptr);
    QString getName() const {return name->text(); }
    QString getSurname() const {return surname->text(); }
    QString getTitle() const {return title->text(); }

 public:
    QPushButton *sendAppointGroup;
    QPushButton *goBack;

 private:
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *titleLabel;
    QLineEdit *name;
    QLineEdit *surname;
    QLineEdit *title;
};

#endif  // APPOINTGROUPWIDGET_H
