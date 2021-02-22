// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "AdminWidget.h"

AdminWidget::AdminWidget(QWidget *parent) : QWidget(parent) {
    results = new QPushButton("View Results", this);
    groups = new QPushButton("View Groups", this);
    tests = new QPushButton("View Planned Tests", this);
    goBack = new QPushButton("Go back", this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(groups);
    layout->addWidget(tests);
    layout->addWidget(results);
    layout->addWidget(goBack);

    setLayout(layout);
}
