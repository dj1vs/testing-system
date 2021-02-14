// Copyright 2021 Dmitriy Trifonov
#include "AllGroupsWidget.h"

AllGroupsWidget::AllGroupsWidget(QWidget *parent, QTableView *t, QStandardItemModel *m) : QWidget(parent), model(m), table(t) {
    goBack = new QPushButton("Go Back", this);
    layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(goBack);

    setLayout(layout);
}

void AllGroupsWidget::showGroupTeachers(QList <QList <QString>> groupTeachers) {
    QTableView *table = new QTableView(this);
    table->setAttribute(Qt::WA_DeleteOnClose);
    QStandardItemModel *model = new QStandardItemModel(groupTeachers.size(), 2, this);
    QList <QString> params = {"Name", "Surname"};
    for (int i = 0; i < 2; ++i) {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for (int row = 0; row < groupTeachers.size(); ++row) {
        for (int col = 0; col < 2; ++col) {
            QModelIndex index = model->index(row, col, QModelIndex());
            model->setData(index, groupTeachers[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    QDialog *d = new QDialog(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(table);
    d->setLayout(v);
    d->exec();
}
void AllGroupsWidget::showGroupStudents(QList <QList <QString>> groupStudents) {
    QTableView *table = new QTableView(this);
    table->setAttribute(Qt::WA_DeleteOnClose);
    QStandardItemModel *model = new QStandardItemModel(groupStudents.size(), 2, this);
    QList <QString> params = {"Name", "Surname"};
    for (int i = 0; i < 2; ++i) {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for (int row = 0; row < groupStudents.size(); ++row) {
        for (int col = 0; col < 2; ++col) {
            QModelIndex index = model->index(row, col, QModelIndex());
            model->setData(index, groupStudents[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    QDialog *d = new QDialog(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(table);
    d->setLayout(v);
    d->exec();
}
