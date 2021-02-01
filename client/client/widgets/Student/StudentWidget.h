#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class StudentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StudentWidget(QWidget *parent = nullptr);
public:
    QPushButton *currentTests;
    QPushButton *results;
    QPushButton *goBack;
signals:

};

#endif // STUDENTWIDGET_H
