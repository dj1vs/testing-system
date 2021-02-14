// Copyright 2021 Dmitriy Trifonov
#ifndef ALLGROUPSWIDGET_H
#define ALLGROUPSWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>

class AllGroupsWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AllGroupsWidget(QWidget *parent = nullptr, QTableView *t = nullptr, QStandardItemModel *m = nullptr);

 public:
    QPushButton *goBack;
    QStandardItemModel *model;
    QList <QPushButton *> list1;
    QList <QPushButton *> list2;

 private:
    QTableView *table;
    QVBoxLayout *layout;
    QList <QString> list;

 public slots:
    void showGroupTeachers(QList <QList <QString>> groupTeachers);
    void showGroupStudents(QList <QList <QString>> groupStudents);
};

#endif  // ALLGROUPSWIDGET_H
