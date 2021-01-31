#include "StudentWidget.h"

StudentWidget::StudentWidget(QWidget *parent) : QWidget(parent)
{
    currentTests = new QPushButton("current tests");
    plannedTests = new QPushButton("planned");
    results = new QPushButton("results");
    goBack = new QPushButton("go back");

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(currentTests);
    l->addWidget(plannedTests);
    l->addWidget(results);
    l->addWidget(goBack);

    setLayout(l);
}
