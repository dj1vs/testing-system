// Copyright 2021 Dmitriy Trifonov
#ifndef TEACHERWIDGET_H
#define TEACHERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class TeacherWidget : public QWidget {
    Q_OBJECT

 public:
    explicit TeacherWidget(QWidget *parent = nullptr);

 public:
    QPushButton *newTaskButton;
    QPushButton *newTestButton;
    QPushButton *viewGroupsButton;
    QPushButton *viewResultsButton;
    QPushButton *appointTest;
    QPushButton *goBack;

 private:
    QVBoxLayout *layout;
};

#endif  // TEACHERWIDGET_H
