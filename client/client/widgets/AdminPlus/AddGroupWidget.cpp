// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "AddGroupWidget.h"

AddGroupWidget::AddGroupWidget(QWidget *parent) : QWidget(parent) {
    titleLabel = new QLabel("Название группы: ", this);
    title = new QLineEdit(this);
    sendGroup = new QPushButton("&Добавить группу", this);
    goBack = new QPushButton("&Назад", this);

    connect(sendGroup, &QPushButton::clicked, this, [this] {emit sendGroupClicked();});
    connect(goBack, &QPushButton::clicked, this, [this] {emit goBackClicked();});

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(title);

    QSpacerItem *spacer = new QSpacerItem(400, 200, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(titleLayout);
    layout->addSpacerItem(spacer);
    layout->addWidget(sendGroup);
    layout->addWidget(goBack);

    this->setLayout(layout);
}
