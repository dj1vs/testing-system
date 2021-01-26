#ifndef ALLRESLULTSWIDGET_H
#define ALLRESLULTSWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class AllReslultsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AllReslultsWidget(QWidget *parent = nullptr);
public:
    QPushButton *allResultsGoBack;
    QPushButton *sortAllResults;
    QPushButton *allResultsSortSave;
private:
    QTableView *allResultsTable;
    QStandardItemModel *allResultsModel;
    QVBoxLayout *viewAllResultsLayout;
    QList <QList<QString>> allResultsList;
    QLabel *allResultsSortNameLabel;
    QLabel *allResultsSortSurnameLabel;
    QLabel *allResultsSortSubjectLabel;
    QLabel *allResultsSortTestLabel;
    QLineEdit *allResultsSortName;
    QLineEdit *allResultsSortSurname;
    QLineEdit *allResultsSortSubject;
    QLineEdit *allResultsSortTest;
    QVBoxLayout *allResultsSortLayout;
signals:

};

#endif // ALLRESLULTSWIDGET_H
