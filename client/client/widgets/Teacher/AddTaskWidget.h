// Copyright 2021 Dmitriy Trifonov
#ifndef ADDTASKWIDGET_H
#define ADDTASKWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QTextEdit>
#include <QStringListModel>
#include <QLineEdit>
#include <QFile>

class AddTaskWidget : public QWidget {
    Q_OBJECT

 public:
    explicit AddTaskWidget(QWidget *parent = nullptr);
    QString getTask() const {return quesition->toPlainText(); }
    QString getAnswer() const {return answer->toPlainText(); }
    QList <QString> getAnswerOptions() const {return answerOptionsModel->stringList(); }
    QString getTheme() const {return theme->text(); }
    QString getSubject() const {return subject->text(); }

 public:
    QPushButton *save;
    QPushButton *quit;

 private:
    QPushButton *newTaskButton;
    QPushButton *newTestButton;
    QPushButton *viewTeacherGroupsButton;
    QPushButton *viewTeacherResultsButton;
    QPushButton *addImage;
    QLabel *quesitionLabel;
    QLabel *answerLabel;
    QLabel *answerOptionsLabel;
    QLabel *themeLabel;
    QLabel *subjectLabel;
    QPushButton *newOption;
    QPushButton *deleteSelected;
    QTextEdit *quesition;
    QTextEdit *answer;
    QStringListModel *answerOptionsModel;
    QListView *answerOptionsView;
    QLineEdit *theme;
    QLineEdit *subject;
    QList <QString> answerOptions;
    QString imageFileName = ":NONE:";

 private:
    void showAddAnswerOptions();
    void answerOptionsDeleteSelected();
};

#endif  // ADDTASKWIDGET_H
