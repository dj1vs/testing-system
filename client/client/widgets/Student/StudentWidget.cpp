// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "StudentWidget.h"

StudentWidget::StudentWidget(QWidget *parent) : QWidget(parent) {
    currentTests = new QPushButton("Текущие тесты");
    results = new QPushButton("Результаты");
    goBack = new QPushButton("Назад");

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(currentTests);
    l->addWidget(results);
    l->addWidget(goBack);

    setLayout(l);
}
