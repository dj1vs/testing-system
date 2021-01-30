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
#include <QTableView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QGroupBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>

typedef enum {RANDOM, MANUAL} STATE;

class AddTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddTestWidget(QWidget *parent = nullptr);

    void setManualTaskList (QList <QList <QString>> l) {manualTaskList = l;};
    void setUserID(int id) {userID = id;};


    QString getName() const {return nameEdit->text();};
    QString getSubject() const {return subjectEdit->text();};
    QDate getDate() const {return dateEdit->selectedDate();};
    int getState() const {return state;};
    QString getTheme() const {return randomThemeEdit->text();};
    int getTasksAmount() const {return randomAmountBox->value();};
    QString getTasksAuthor() const {return randomShowMy->isChecked() ? "ME" : "ALL";};
    QList <QString> getPickedTasks() const {
        QList <QString> res;
        for(int i = 0; i < manualPickedModel->rowCount(); ++i)
            res.append(manualAllModel->data(manualAllModel->index(i,0,QModelIndex())).toString());
        return res;
    };

    void setManual();
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

    QLabel *manualAllLabel;
    QLabel *manualPickedLabel;
    QTableView *manualAll;
    QStandardItemModel *manualAllModel;
    QStandardItemModel *manualPickedModel;
    QTableView *manualPicked;

    QLineEdit *manualSortTask;
    QComboBox *manualSortSubject;
    QComboBox *manualSortTheme;
    QCheckBox *manualSortMine;

    QSet <QString> manualSubjects;
    QSet <QString> manualThemes;
    QList <QList <QString>> manualTaskList;
    QPushButton *manualSort;
    QPushButton *manualDelete;
    QPushButton *manualPick;
    QPushButton *manualUp;
    QPushButton *manualDown;
    QDialogButtonBox *manualButtonBox;
    QVBoxLayout *manualLayout;

    int userID = 1;




private:
    bool checkStart();
    void setParamsRandom();
signals:
    void finished();
    void setUpManual();

};

#endif // ADDTESTWIDGET_H
