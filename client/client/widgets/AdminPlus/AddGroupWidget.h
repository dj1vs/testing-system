#ifndef ADDGROUPWIDGET_H
#define ADDGROUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AddGroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddGroupWidget(QWidget *parent = nullptr);
    QString getGroupTitle() const {return title->text();};

public:
    QPushButton *sendGroup;
    QPushButton *goBack;
private:
    QLabel *titleLabel;
    QLineEdit *title;
    QHBoxLayout *titleLayout;
    QVBoxLayout *addGroupLayout;

signals:

};

#endif // ADDGROUPWIDGET_H
