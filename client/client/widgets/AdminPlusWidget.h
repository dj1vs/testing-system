#ifndef ADMINPLUSWIDGET_H
#define ADMINPLUSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class AdminPlusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminPlusWidget(QWidget *parent = nullptr);
    QPushButton *addGroup;
    QPushButton *addToGroup;
    QPushButton *appointGroup;
    QPushButton *addUser;
    QVBoxLayout *layout;

signals:
};

#endif // ADMINPLUSWIDGET_H
