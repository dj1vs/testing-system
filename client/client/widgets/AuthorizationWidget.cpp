// Copyright 2021 Dmitriy Trifonov
#include <QFormLayout>
#include "AuthorizationWidget.h"

AuthorizationWidget::AuthorizationWidget(QWidget *parent) : QWidget(parent) {
    login = new QLineEdit(this);
    password = new KPasswordLineEdit();

    authorize = new QPushButton("&Log in", this);
    QFormLayout *f = new QFormLayout();
    f->addRow(tr("Login"), login);
    f->addRow(tr("Password"), password);
    f->addWidget(authorize);

    setLayout(f);
}
