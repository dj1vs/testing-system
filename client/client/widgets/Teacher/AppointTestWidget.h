// Copyright 2021 Dmitriy Trifonov
#ifndef APPOINTTESTWIDGET_H
#define APPOINTTESTWIDGET_H

#include <QWidget>
#include <KComboBox>
#include <KCompletion>
#include <QPushButton>

class AppointTestWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AppointTestWidget(QWidget *parent = nullptr, QList <QString> groups = {}, QList <QString> tests = {});
    QString getGroup() const {return groupBox->currentText(); }
    QString getTest() const {return testBox->currentText(); }

 public:
    QPushButton *submit;
    QPushButton *goBack;

 private:
    KComboBox *groupBox;
    KComboBox *testBox;

    KCompletion *groupCompletion;
    KCompletion *testCompletion;
};

#endif  // APPOINTTESTWIDGET_H
