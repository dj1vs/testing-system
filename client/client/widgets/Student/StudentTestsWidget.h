#ifndef STUDENTTESTSWIDGET_H
#define STUDENTTESTSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>

class StudentTestsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StudentTestsWidget(QWidget *parent = nullptr);
public:
    QPushButton *goBack;
    QPushButton *startTest;
private:
    QTableView *table;
    QStandardItemModel *model;

signals:

};

#endif // STUDENTTESTSWIDGET_H
