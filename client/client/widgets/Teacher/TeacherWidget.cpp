// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "TeacherWidget.h"

TeacherWidget::TeacherWidget(QWidget *parent) : QWidget(parent) {
    newTaskButton = new QPushButton("Новое задание");
    newTestButton = new QPushButton("Новый тест");
    viewGroupsButton = new QPushButton("Мои группы");
    viewResultsButton = new QPushButton("Результаты моих учеников");
    appointTest = new QPushButton("Задать тест");
    goBack = new QPushButton("Назад");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(newTaskButton);
    layout->addWidget(newTestButton);
    layout->addWidget(viewGroupsButton);
    layout->addWidget(viewResultsButton);
    layout->addWidget(appointTest);
    layout->addWidget(goBack);

    setLayout(layout);
}
