// Copyright 2021 Dmitriy Trifonov
#include <QDialog>
#include <QVBoxLayout>
#include "TeacherGroupsWidget.h"

TeacherGroupsWidget::TeacherGroupsWidget(QWidget *parent, QList <QString> list) : QWidget(parent) {
    goBack = new QPushButton("GoBack");
    view = new QTableView();
    model = new QStandardItemModel(list.size(), 2);
    for (int i = 0; i < 2; ++i) {
        QByteArray ba = headerList[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for (int row = 0; row < list.size(); ++row) {
        QModelIndex index = model->index(row, 0, QModelIndex());
        model->setData(index, list[row]);
    }
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setModel(model);


    for (int row = 0; row < list.size(); ++row) {
        QModelIndex index = model->index(row, 1, QModelIndex());
        QPushButton *buf = new QPushButton(this);
        buf->setAccessibleName(list[row]);
        tableButtons.push_back(buf);
        view->setIndexWidget(index, buf);
    }

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(view);
    l->addWidget(goBack);
    setLayout(l);
}
void TeacherGroupsWidget::showGroupStudents(QList <QList <QString>> groupStudents) {
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
    d->show();
}
