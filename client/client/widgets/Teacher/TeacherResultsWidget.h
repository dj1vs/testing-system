#ifndef TEACHERRESULTSWIDGET_H
#define TEACHERRESULTSWIDGET_H

#include "../../lib/DateConverter.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QDialog>
#include <QDebug>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QDialogButtonBox>

class TeacherResultsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeacherResultsWidget(QWidget *parent = nullptr, QList <QList <QString>> l = {});
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

    const QList <QString> params = {"name","surname", "group", "test name", "percent", "subject", "date"};

    QList <QList <QString>> list;
    QSet <QList<QString>> students;
    QSet <QString> groups;
    QSet <QString> testNames;
    QSet <QString> subjects;
private:
    void showSort();
    void editTable();
signals:

};

#endif // TEACHERRESULTSWIDGET_H
