#ifndef TEACHERGROUPSWIDGET_H
#define TEACHERGROUPSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QDialog>
#include <QDebug>


class TeacherGroupsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeacherGroupsWidget(QWidget *parent = nullptr, QList <QString> list = {});
    void showGroupStudents(QList <QList <QString>> groupStudents);
signals:
public:
    QPushButton *goBack;
    QList <QPushButton *> tableButtons;
private:
    QTableView *view;
    QStandardItemModel *model;

    const QList <QString> headerList = {"Group name", "students"};

};

#endif // TEACHERGROUPSWIDGET_H
