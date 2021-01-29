#include "AddTaskWidget.h"

AddTaskWidget::AddTaskWidget(QWidget *parent) : QWidget(parent)
{
    quesitionLabel = new QLabel("Вопрос:");
    answerLabel = new QLabel("Answer");
    answerOptionsLabel = new QLabel("Answer options");
    themeLabel = new QLabel("Theme");
    subjectLabel = new QLabel("Subject");

    save = new QPushButton("save");
    quit = new QPushButton("quit");
    newOption = new QPushButton("add new answer option");
    deleteEmpty = new QPushButton("delete empty");

    connect(newOption, &QPushButton::clicked, this,
            [this] {showAddAnswerOptions();});

    connect(deleteEmpty, &QPushButton::clicked, this,
            [this] {addTaskAnswerOptionsDeleteEmpty();});

    quesition = new QTextEdit();
    answer = new QTextEdit();

    answerOptionsModel = new QStringListModel();
    answerOptionsView = new QListView();

    theme = new QLineEdit();
    subject = new QLineEdit();

    layout = new QVBoxLayout();
    layout->addWidget(quesitionLabel);
    layout->addWidget(quesition);
    layout->addWidget(answerOptionsLabel);
    layout->addWidget(answerOptionsView);
    layout->addWidget(deleteEmpty);
    layout->addWidget(newOption);
    layout->addWidget(answerLabel);
    layout->addWidget(answer);
    layout->addWidget(subjectLabel);
    layout->addWidget(subject);
    layout->addWidget(themeLabel);
    layout->addWidget(theme);
    layout->addWidget(save);
    layout->addWidget(quit);

    setLayout(layout);
}

void AddTaskWidget::showAddAnswerOptions()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New answer option:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        answerOptions.push_back(text);
        answerOptionsModel->setStringList(answerOptions);
        answerOptionsView->setModel(answerOptionsModel);
    }
}
void AddTaskWidget::addTaskAnswerOptionsDeleteEmpty()
{
    for(int i = 0; i < answerOptionsModel->rowCount(); ++i)
    {
        if(answerOptionsModel->stringList().at(i) == "")
            answerOptionsModel->removeRow(i);
    }
    answerOptionsView->setModel(answerOptionsModel);
}
