// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "StudentResultsWidget.h"

StudentResultsWidget::StudentResultsWidget(QList <QStringList> new_list, QWidget *parent) : QWidget(parent), list(new_list) {
    const QStringList params = {"Предмет", "Тест", "Дата теста", "Процента"};

    QTableView *table = new QTableView();
    QStandardItemModel *model = new QStandardItemModel(list.size(), params.size(), nullptr);

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

    QPushButton *goBack = new QPushButton("назад");
    connect(goBack, &QPushButton::clicked, this, [this] {emit finished(); });

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(table);
    l->addWidget(goBack);

    setLayout(l);
}
