// Copyright 2021 Dmitriy Trifonov
#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include <QWidget>
#include <QPushButton>

class StudentWidget : public QWidget {
    Q_OBJECT

 public:
    explicit StudentWidget(QWidget *parent = nullptr);

 public:
    QPushButton *currentTests;
    QPushButton *results;
    QPushButton *goBack;
};

#endif  // STUDENTWIDGET_H
