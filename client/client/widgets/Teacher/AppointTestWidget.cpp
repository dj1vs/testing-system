// Copyright 2021 Dmitriy Trifonov
#include <QFormLayout>
#include "AppointTestWidget.h"

AppointTestWidget::AppointTestWidget(QWidget *parent, QList <QString> groups, QList <QString> tests) : QWidget(parent) {
    groupBox = new KComboBox(true, this);
    groupBox->insertItems(0, groups);

    groupCompletion = groupBox->completionObject();

    // Connect to the return pressed signal - optional
    connect(groupBox, SIGNAL(returnPressed(QString)), groupCompletion, SLOT(addItem(QString)));

    groupCompletion->insertItems(groups);


    testBox = new KComboBox(true, this);
    testBox->insertItems(0, tests);

    testCompletion = testBox->completionObject();

    // Connect to the return pressed signal - optional
    connect(testBox, SIGNAL(returnPressed(QString)), testCompletion, SLOT(addItem(QString)));

    testCompletion->insertItems(groups);
    submit = new QPushButton("Подтвердить");
    goBack = new QPushButton("Назад");

    QFormLayout *l = new QFormLayout();
    l->addRow(tr("Группа:"), groupBox);
    l->addRow(tr("Тест:"), testBox);
    l->addWidget(submit);
    l->addWidget(goBack);

    setLayout(l);
}
