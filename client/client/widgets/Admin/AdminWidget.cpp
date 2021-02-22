// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "AdminWidget.h"

AdminWidget::AdminWidget(QWidget *parent) : QWidget(parent) {
    results = new QPushButton("Результаты", this);
    groups = new QPushButton("Группы", this);
    tests = new QPushButton("Тесты", this);
    goBack = new QPushButton("Назад", this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(groups);
    layout->addWidget(tests);
    layout->addWidget(results);
    layout->addWidget(goBack);

    setLayout(layout);
}
