// Copyright 2021 Dmitriy Trifonov
#include <QFormLayout>
#include "AuthorizationWidget.h"

AuthorizationWidget::AuthorizationWidget(QWidget *parent) : QWidget(parent) {
    login = new QLineEdit(this);
    password = new KPasswordLineEdit();

    authorize = new QPushButton("&Войти", this);
    QFormLayout *f = new QFormLayout();
    f->addRow(tr("Логин"), login);
    f->addRow(tr("Пароль"), password);
    f->addWidget(authorize);

    setLayout(f);
}
