// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include <QFormLayout>
#include "AppointGroupWidget.h"

AppointGroupWidget::AppointGroupWidget(QWidget *parent) : QWidget(parent) {
    nameLabel = new QLabel("Teacher name:", this);
    surnameLabel = new QLabel("Teacher surname:", this);
    titleLabel = new QLabel("Group name:", this);
    name = new QLineEdit();
    surname = new QLineEdit();
    title = new QLineEdit();
    sendAppointGroup = new QPushButton("Appoint group", this);
    goBack = new QPushButton("Go back", this);

    QFormLayout *f = new QFormLayout();
    f->addRow(nameLabel, name);
    f->addRow(surnameLabel, surname);
    f->addRow(titleLabel, title);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(f);
    layout->addWidget(sendAppointGroup);
    layout->addWidget(goBack);

    setLayout(layout);
}
