// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "StudentTestsWidget.h"

StudentTestsWidget::StudentTestsWidget(QList <QStringList> list,  QWidget *parent) : QWidget(parent) {
    goBack = new QPushButton("Назад", this);
    start = new QPushButton("Старт", this);

    table = new QTableView(this);
    model = new QStandardItemModel(list.size(), params.size(), this);

    for (int i = 0; i < params.size(); ++i) {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for (int row = 0; row < list.size(); ++row) {
        for (int col = 0; col < params.size(); ++col) {
            QModelIndex index = model->index(row, col, QModelIndex());
            model->setData(index, list[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(table);
    l->addWidget(start);
    l->addWidget(goBack);

    setLayout(l);
}
