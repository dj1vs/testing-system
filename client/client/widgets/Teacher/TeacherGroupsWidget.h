// Copyright 2021 Dmitriy Trifonov
#ifndef TEACHERGROUPSWIDGET_H
#define TEACHERGROUPSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>


class TeacherGroupsWidget : public QWidget {
    Q_OBJECT

 public:
    explicit TeacherGroupsWidget
    (QWidget *parent = nullptr, QList <QString> list = {});
    void showGroupStudents(QList <QList <QString>> groupStudents);

 public:
    QPushButton *goBack;
    QList <QPushButton *> tableButtons;

 private:
    QTableView *view;
    QStandardItemModel *model;

    const QList <QString> headerList = {"Группа", "Ученики"};
};

#endif  // TEACHERGROUPSWIDGET_H
