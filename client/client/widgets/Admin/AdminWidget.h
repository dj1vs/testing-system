// Copyright 2021 Dmitriy Trifonov
#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QPushButton>

class AdminWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AdminWidget(QWidget *parent = nullptr);

 public:
    QPushButton *results;
    QPushButton *groups;
    QPushButton *tests;
    QPushButton *goBack;
};

#endif  // ADMINWIDGET_H
