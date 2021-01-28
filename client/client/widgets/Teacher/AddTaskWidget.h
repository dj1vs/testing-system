#ifndef ADDTASKWIDGET_H
#define ADDTASKWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QStringListModel>
#include <QInputDialog>

class AddTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddTaskWidget(QWidget *parent = nullptr);
    QString getTask() const {return addTaskQuesition->toPlainText();};
    QString getAnswer() const {return addTaskAnswer->toPlainText();};
    QList <QString> getAnswerOptions() const {return addTaskAnswerOptionsModel->stringList();};
    QString getTheme() const {return addTaskTheme->text();};
    QString getSubject() const {return addTaskSubject->text();};

public:
    QPushButton *addTaskSave;
    QPushButton *addTaskQuit;
private:
    QPushButton *newTaskButton;
    QPushButton *newTestButton;
    QPushButton *viewTeacherGroupsButton;
    QPushButton *viewTeacherResultsButton;
    QVBoxLayout *teacherWindowLayout;
    QLabel *addTaskQuesitionLabel;
    QLabel *addTaskAnswerLabel;
    QLabel *addTaskAnswerOptionsLabel;
    QLabel *addTaskThemeLabel;
    QLabel *addTaskSubjectLabel;
    QPushButton *addTaskNewOption;
    QPushButton *addTaskDeleteEmpty;
    QTextEdit *addTaskQuesition;
    QTextEdit *addTaskAnswer;
    QStringListModel *addTaskAnswerOptionsModel;
    QListView *addTaskAnswerOptionsView;
    QLineEdit *addTaskTheme;
    QLineEdit *addTaskSubject;
    QVBoxLayout *addTaskLayout;
    QList <QString> addTaskAnswerOptions;
private:
    void showAddAnswerOptions();
    void addTaskAnswerOptionsDeleteEmpty();
signals:

};

#endif // ADDTASKWIDGET_H
