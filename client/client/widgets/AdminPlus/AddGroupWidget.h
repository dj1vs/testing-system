// Copyright 2021 Dmitriy Trifonov
#ifndef ADDGROUPWIDGET_H
#define ADDGROUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class AddGroupWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AddGroupWidget(QWidget *parent = nullptr);
    QString getGroupTitle() const {return title->text(); }

 private:
    QLabel *titleLabel;
    QLineEdit *title;
    QPushButton *sendGroup;
    QPushButton *goBack;

 signals:
    void goBackClicked();
    void sendGroupClicked();
};

#endif  // ADDGROUPWIDGET_H
