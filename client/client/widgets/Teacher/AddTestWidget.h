#ifndef ADDTESTWIDGET_H
#define ADDTESTWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QCalendarWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QDebug>
#include "../custom/EditListBox.h"

typedef enum {RANDOM, MANUAL} STATE;

class AddTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddTestWidget(QWidget *parent = nullptr);
    QString getName() const {return nameEdit->text();};
    QString getSubject() const {return subjectEdit->text();};
    QDate getDate() const {return dateEdit->selectedDate();};
    int getState() const {return state;};
    QString getTheme() const {return randomThemeEdit->text();};
    int getTasksAmount() const {return randomAmountBox->value();};
    QString getTasksAuthor() const {return randomShowMy->isChecked() ? "ME" : "ALL";};
public:
    STATE state;
    QPushButton *createButton;
private:
    QList <QLabel *> labels;
    const QList <QString> labelNames = {"Test name", "Subject", "Date"};
    QLineEdit *nameEdit;
    QLineEdit *subjectEdit;
    QCheckBox *isManualCheck;
    QCalendarWidget *dateEdit;
    QVBoxLayout *layout;

    QLabel *randomThemeLabel;
    QLabel *randomAmountLabel;
    QLineEdit *randomThemeEdit;
    QCheckBox *randomShowMy;
    QSpinBox *randomAmountBox;
    QDialogButtonBox *randomButtonBox;
    QVBoxLayout *randomLayout;

    EditListBox *test;



private:
    bool checkStart();
    void setParamsRandom();
signals:
    void finished();

};

#endif // ADDTESTWIDGET_H
