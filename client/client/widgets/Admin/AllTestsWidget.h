#ifndef ALLTESTSWIDGET_H
#define ALLTESTSWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include <QDateEdit>
#include <QRadioButton>
#include <QTextBrowser>
#include <QStringListModel>
#include <QListView>
#include <QErrorMessage>

class AllTestsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AllTestsWidget(QWidget *parent = nullptr, QList<QList <QString>> l = {});
    void showTestTasks(QList <QList<QString>> allPlannedTestsTaskList);
public:
    QPushButton *goBack;
    QList <QPushButton *> buttons;
private:
    QTableView *table;
    QStandardItemModel *model;
    QPushButton *sort;
    QVBoxLayout *layout;

    QList <QPushButton *> list;
    ////////////////////////////////////////////
    QList <QList<QString>> taskList;
    QList <QList <QString>> testsList;
    QLabel *sortNameLabel;
    QLabel *sortSurnameLabel;
    QLabel *sortTestLabel;
    QLabel *sortDateLabel;
    QLabel *sortSubjectLabel;
    QLineEdit *sortName;
    QLineEdit *sortSurname;
    QLineEdit *sortTest;
    QDateEdit *sortDate;
    QLineEdit *sortSubject;
    QRadioButton *sortViewPast;
    QRadioButton *sortViewFuture;
    QRadioButton *sortViewAll;
    QPushButton *sortSave;
    QVBoxLayout *sortLayout;
    quint16 tasksAmount = 0;
    quint16 currTask = 0;
    QPushButton *taskNext;
    QPushButton *taskPrev;
    QTextBrowser *taskText;
    QTextBrowser *taskAnswer;
    QStringListModel *taskAnswerOptionsModel;
    QListView *taskAnswerOptionsView;
    QVBoxLayout *taskLayout;
private:
    void showAllPlannedTestsSort();
    void editAllPlannedTestsTable();
    void showNextTask();
    void showPrevTask();
signals:

};

#endif // ALLTESTSWIDGET_H