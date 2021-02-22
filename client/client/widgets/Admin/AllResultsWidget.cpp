// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include <QDialog>
#include "AllResultsWidget.h"

AllResultsWidget::AllResultsWidget(QWidget *parent, QList <QList <QString>> l) : QWidget(parent), list(l) {
    goBack = new QPushButton("Назад", this);
    sort = new QPushButton("Редактировать параметры сортировки", this);

    connect(sort, &QPushButton::clicked, this,
            [this] () {showAllResultsSort(); });

    table = new QTableView();
    model = new QStandardItemModel(list.size(), 5, this);

    QList <QString> params = {"Тест", "Предмет", "Имя", "Фамилия", "Процент"};

    for (int i = 0; i < 5; ++i) {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    for (int row = 0; row < list.size(); ++row) {
        for (int col = 0; col < 5; ++col) {
            QModelIndex index = model->index(row, col, QModelIndex());
            model->setData(index, list[row][col]);
        }
    }

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(sort);
    layout->addWidget(goBack);

    setLayout(layout);
}
void AllResultsWidget::showAllResultsSort() {
    sortNameLabel = new QLabel("Имя:", this);
    sortSurnameLabel = new QLabel("Фамилия:", this);
    sortSubjectLabel = new QLabel("Предмет:", this);
    sortTestLabel = new QLabel("Тест:", this);

    sortName = new QLineEdit(this);
    sortSurname = new QLineEdit(this);
    sortSubject = new QLineEdit(this);
    sortTest = new QLineEdit(this);

    save = new QPushButton("Сохранить", this);

    QHBoxLayout *name = new QHBoxLayout();
    name->addWidget(sortNameLabel);
    name->addWidget(sortName);
    QHBoxLayout *surname = new QHBoxLayout();
    surname->addWidget(sortSurnameLabel);
    surname->addWidget(sortSurname);
    QHBoxLayout *subject = new QHBoxLayout();
    subject->addWidget(sortSubjectLabel);
    subject->addWidget(sortSubject);
    QHBoxLayout *test = new QHBoxLayout();
    test->addWidget(sortTestLabel);
    test->addWidget(sortTest);

    QVBoxLayout *sortLayout = new QVBoxLayout();
    sortLayout->addLayout(name);
    sortLayout->addLayout(surname);
    sortLayout->addLayout(subject);
    sortLayout->addLayout(test);
    sortLayout->addWidget(save);

    QDialog *d = new QDialog(this);
    d->setLayout(sortLayout);
    d->show();

    connect(save, &QPushButton::clicked, this,
            [this, d] () {editAllResults(); d->close(); });
}

void AllResultsWidget::editAllResults() {
    QString name = sortName->text();
    QString surname = sortSurname->text();
    QString subject = sortSubject->text();
    QString test = sortTest->text();

    if (name == "" && surname == "" && subject == "" && test == "") {
        model->removeRows(0, model->rowCount());
        model->setRowCount(list.size());
        for (int row = 0; row < list.size(); ++row) {
            for (int col = 0; col < 5; ++col) {
                QModelIndex index = model->index(row, col, QModelIndex());
                model->setData(index, list[row][col]);
            }
        }
        table->setModel(model);
    }

    for (int row = 0; row < model->rowCount(); ++row) {
        QString modelName = model->index(row, 2, QModelIndex()).data().toString();
        QString modelSurname = model->index(row, 3, QModelIndex()).data().toString();
        QString modelSubject = model->index(row, 1, QModelIndex()).data().toString();
        QString modelTest = model->index(row, 0, QModelIndex()).data().toString();

        if ((name != "" && surname != "" && (modelName != name || modelSurname != surname))
                || (subject != "" && modelSubject != subject) || (test != "" && modelTest != test)) {
            model->removeRow(row);
            row--;
        }
    }
    table->setModel(model);
}
