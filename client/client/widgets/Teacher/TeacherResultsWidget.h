// Copyright 2021 Dmitriy Trifonov
#ifndef TEACHERRESULTSWIDGET_H
#define TEACHERRESULTSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStandardItemModel>
#include <QTableView>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QDialogButtonBox>

#include "../../lib/DateConverter.h"

class TeacherResultsWidget : public QWidget {
    Q_OBJECT

 public:
    explicit TeacherResultsWidget(QWidget *parent = nullptr,
                                  QList <QList <QString>> l = {});

 public:
    QPushButton *goBack;

 private:
    QPushButton *sort;
    QStandardItemModel *model;
    QTableView *table;

    QComboBox *studentBox;
    QComboBox *groupBox;
    QComboBox *testnameBox;
    QComboBox *subjectBox;
    QCheckBox *sortByDate;
    QDateEdit *sortDateEdit;
    QDialogButtonBox *buttonBox;

    const QList <QString> params =
    {"name", "surname", "group", "test name", "percent", "subject", "date"};

    QList <QList <QString>> list;
    QList <QList<QString>> students;
    QList <QString> groups;
    QList <QString> testNames;
    QList <QString> subjects;

 private:
    void showSort();
    void editTable();
};

#endif  // TEACHERRESULTSWIDGET_H
