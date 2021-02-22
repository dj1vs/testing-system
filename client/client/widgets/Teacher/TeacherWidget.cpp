// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "TeacherWidget.h"

TeacherWidget::TeacherWidget(QWidget *parent) : QWidget(parent) {
    newTaskButton = new QPushButton("new task");
    newTestButton = new QPushButton("new test");
    viewGroupsButton = new QPushButton("view groups");
    viewResultsButton = new QPushButton("view results");
    appointTest = new QPushButton("appoint test");
    goBack = new QPushButton("go back");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(newTaskButton);
    layout->addWidget(newTestButton);
    layout->addWidget(viewGroupsButton);
    layout->addWidget(viewResultsButton);
    layout->addWidget(appointTest);
    layout->addWidget(goBack);

    setLayout(layout);
}
