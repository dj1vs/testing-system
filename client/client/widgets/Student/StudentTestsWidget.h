// Copyright 2021 Dmitriy Trifonov
#ifndef STUDENTTESTSWIDGET_H
#define STUDENTTESTSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>

class StudentTestsWidget : public QWidget {
    Q_OBJECT

 public:
    explicit StudentTestsWidget(QList <QStringList> list,  QWidget *parent = nullptr);
    QString getSelectedTest() const {
        if (!table->selectionModel()->hasSelection())
            return "";
        int row = table->selectionModel()->selectedRows()[0].row();
         return model->data(model->index(row, 0, QModelIndex())).toString();}

 public:
    QPushButton *goBack;
    QPushButton *start;

 private:
    QTableView *table;
    QStandardItemModel *model;
    const QStringList params = {"test name", "subject", "planned date"};
};

#endif  // STUDENTTESTSWIDGET_H
