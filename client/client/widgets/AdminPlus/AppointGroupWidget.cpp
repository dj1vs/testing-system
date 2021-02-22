// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include <QFormLayout>
#include "AppointGroupWidget.h"

AppointGroupWidget::AppointGroupWidget(QWidget *parent) : QWidget(parent) {
    nameLabel = new QLabel("Имя учителя:", this);
    surnameLabel = new QLabel("Фамилия учителя:", this);
    titleLabel = new QLabel("Название группы:", this);
    name = new QLineEdit();
    surname = new QLineEdit();
    title = new QLineEdit();
    sendAppointGroup = new QPushButton("Назначить группу", this);
    goBack = new QPushButton("Назад", this);

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
