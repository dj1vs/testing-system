// Copyright 2021 Dmitriy Trifonov
#include <QFormLayout>
#include <QVBoxLayout>
#include "AddToGroupWidget.h"

AddToGroupWidget::AddToGroupWidget(QWidget *parent) : QWidget(parent) {
    nameLabel = new QLabel("Name:", this);
    surnameLabel = new QLabel("Surname:", this);
    titleLabel = new QLabel("Group title:", this);
    name = new QLineEdit(this);
    surname = new QLineEdit(this);
    title = new QLineEdit(this);
    sendToGroup = new QPushButton("&Add", this);
    goBack = new QPushButton("&Go back", this);

    QFormLayout *fl = new QFormLayout();
    fl->addRow(nameLabel, name);
    fl->addRow(surnameLabel, surname);
    fl->addRow(titleLabel, title);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(fl);
    layout->addWidget(sendToGroup);
    layout->addWidget(goBack);

    setLayout(layout);
}
