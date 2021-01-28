#include "TeacherWidget.h"

TeacherWidget::TeacherWidget(QWidget *parent) : QWidget(parent)
{
    newTaskButton = new QPushButton("new task");
    newTestButton = new QPushButton("new test");
    viewGroupsButton = new QPushButton("view groups");
    viewResultsButton = new QPushButton("view results");
    goBack = new QPushButton("go back");

    layout = new QVBoxLayout();
    layout->addWidget(newTaskButton);
    layout->addWidget(newTestButton);
    layout->addWidget(viewGroupsButton);
    layout->addWidget(viewResultsButton);
    layout->addWidget(goBack);

    setLayout(layout);
}
