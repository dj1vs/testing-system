#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AdminWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminWidget(QWidget *parent = nullptr);
public:
    QPushButton *results;
    QPushButton *groups;
    QPushButton *tests;
    QPushButton *goBack;
private:
    QVBoxLayout *layout;
signals:

};

#endif // ADMINWIDGET_H
