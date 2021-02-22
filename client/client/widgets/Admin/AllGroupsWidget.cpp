// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "AllGroupsWidget.h"

AllGroupsWidget::AllGroupsWidget(QWidget *parent, QList <QStringList> l) : QWidget(parent), list(l) {
    table = new QTableView();
    model = new QStandardItemModel(list.size(), 3, this);

    QList <QString> params = {"Name", "Teachers", "Students"};

    for (int i = 0; i < 3; ++i) {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    for (int row = 0; row < list.size(); ++row) {
        for (int col = 0; col < 2; ++col) {
        QModelIndex index = model->index(row, col, QModelIndex());
        model->setData(index, list[row][col]);
        }
    }

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    for (int row = 0; row < list.size(); ++row) {
        QString str = model->index(row, 0, QModelIndex()).data().toString();
        QModelIndex index = model->index(row, 2, QModelIndex());
        QPushButton *button = new QPushButton(this);
        button->setAccessibleName(str);
        buttonsList.push_back(button);

//        connect(button, &QPushButton::clicked, this,
//                [this, button] () {slotSendToServer("{cmd='view group students';groupname='" + button->accessibleName() + "';}"); });


        table->setIndexWidget(index, button);
    }
    goBack = new QPushButton("Go Back", this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(goBack);

    setLayout(layout);
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
