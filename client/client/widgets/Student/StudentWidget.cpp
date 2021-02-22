// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "StudentWidget.h"

StudentWidget::StudentWidget(QWidget *parent) : QWidget(parent) {
    currentTests = new QPushButton("current tests");
    results = new QPushButton("results");
    goBack = new QPushButton("go back");

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(currentTests);
    l->addWidget(results);
    l->addWidget(goBack);

    setLayout(l);
}
