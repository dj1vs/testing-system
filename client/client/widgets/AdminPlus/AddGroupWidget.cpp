#include "AddGroupWidget.h"

AddGroupWidget::AddGroupWidget(QWidget *parent) : QWidget(parent)
{
    titleLabel = new QLabel("Group title: ", this);
    title = new QLineEdit(this);
    sendGroup = new QPushButton("&Add group", this);
    goBack = new QPushButton("&Go back", this);

    titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(title);

    addGroupLayout = new QVBoxLayout();
    addGroupLayout->addLayout(titleLayout);
    addGroupLayout->addWidget(sendGroup);
    addGroupLayout->addWidget(goBack);

    this->setLayout(addGroupLayout);
}
