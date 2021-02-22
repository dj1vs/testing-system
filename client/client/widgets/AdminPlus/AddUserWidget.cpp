// Copyright 2021 Dmitriy Trifonov
#include <QFormLayout>
#include "AddUserWidget.h"

AddUserWidget::AddUserWidget(QWidget *parent) : QWidget(parent) {
    loginLabel = new QLabel("Login:", this);
    passwordLabel = new QLabel("Password:", this);
    nameLabel = new QLabel("Name:", this);
    surnameLabel = new QLabel("Surname:", this);
    roleLabel = new QLabel("Role:", this);

    roleBox = new QComboBox(this);
    roleBox->addItems({"admin+", "admin", "teacher", "student"});

    login = new QLineEdit(this);
    password = new QLineEdit(this);
    name = new QLineEdit(this);
    surname = new QLineEdit(this);
    addUser = new QPushButton("Add user", this);
    goBack = new QPushButton("Go back", this);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(loginLabel, login);
    layout->addRow(passwordLabel, password);
    layout->addRow(nameLabel, name);
    layout->addRow(surnameLabel, surname);
    layout->addRow(roleLabel, roleBox);
    layout->addWidget(addUser);
    layout->addWidget(goBack);

    setLayout(layout);
}
