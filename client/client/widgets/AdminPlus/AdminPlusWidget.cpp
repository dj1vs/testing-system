// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include "AdminPlusWidget.h"

AdminPlusWidget::AdminPlusWidget(QWidget *parent) : QWidget(parent) {
    addGroup = new QPushButton("&Добавить группу", this);
    addToGroup = new QPushButton("&Добавить в группу", this);
    appointGroup = new QPushButton("&Назначить группу", this);
    addUser = new QPushButton("&Добавить пользователя", this);
    goBack = new QPushButton("&Назад", this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(addGroup);
    layout->addWidget(addToGroup);
    layout->addWidget(appointGroup);
    layout->addWidget(addUser);
    layout->addWidget(goBack);

    setLayout(layout);
}
