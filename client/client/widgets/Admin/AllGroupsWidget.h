// Copyright 2021 Dmitriy Trifonov
#ifndef ALLGROUPSWIDGET_H
#define ALLGROUPSWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>

class AllGroupsWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AllGroupsWidget(QWidget *parent = nullptr, QList <QStringList> l = {});

 public:
    QPushButton *goBack;
    QList <QPushButton *> buttonsList;

 private:
    QTableView *table;
    QStandardItemModel *model;
    QList <QStringList> list;

 public slots:
    void showGroupStudents(QList <QList <QString>> groupStudents);
};

#endif  // ALLGROUPSWIDGET_H
