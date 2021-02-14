// Copyright 2021 Dmitriy Trifonov
#ifndef ADMINPLUSWIDGET_H
#define ADMINPLUSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class AdminPlusWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AdminPlusWidget(QWidget *parent = nullptr);
    QPushButton *addGroup;
    QPushButton *addToGroup;
    QPushButton *appointGroup;
    QPushButton *goBack;
    QPushButton *addUser;

 private:
    QVBoxLayout *layout;
};

#endif  // ADMINPLUSWIDGET_H
