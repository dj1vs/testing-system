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

class AllGroupsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AllGroupsWidget(QWidget *parent = nullptr, QList <QString> l = {});
public:
    QPushButton *goBack;
    QStandardItemModel *model;
    QList <QPushButton *> list1;
    QList <QPushButton *> list2;
private:
    QTableView *table;
    QVBoxLayout *layout;
    QList <QString> list;
private:
    bool isTeachersOpened;
    bool isStudentsOpened;
public slots:
    void showGroupTeachers(QList <QList <QString>> groupTeachers);
    void showGroupStudents(QList <QList <QString>> groupStudents);

signals:

};

#endif // ALLGROUPSWIDGET_H
