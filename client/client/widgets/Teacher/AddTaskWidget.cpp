#include "AddTaskWidget.h"

AddTaskWidget::AddTaskWidget(QWidget *parent) : QWidget(parent)
{
    addTaskAnswerOptions.clear();
    addTaskQuesitionLabel = new QLabel("Вопрос:");
    addTaskAnswerLabel = new QLabel("Answer");
    addTaskAnswerOptionsLabel = new QLabel("Answer options");
    addTaskThemeLabel = new QLabel("Theme");
    addTaskSubjectLabel = new QLabel("Subject");

    addTaskSave = new QPushButton("save");
    addTaskQuit = new QPushButton("quit");
    addTaskNewOption = new QPushButton("add new answer option");
    addTaskDeleteEmpty = new QPushButton("delete empty");

//    connect(addTaskSave, SIGNAL(clicked()), this, SLOT(sendTaskToSystem()));

    connect(addTaskNewOption, &QPushButton::clicked, this,
            [this] {showAddAnswerOptions();});

    connect(addTaskDeleteEmpty, &QPushButton::clicked, this,
            [this] {addTaskAnswerOptionsDeleteEmpty();});

    addTaskQuesition = new QTextEdit();
    addTaskAnswer = new QTextEdit();

    addTaskAnswerOptionsModel = new QStringListModel();
    addTaskAnswerOptionsView = new QListView();

    addTaskTheme = new QLineEdit();
    addTaskSubject = new QLineEdit();

    addTaskLayout = new QVBoxLayout();
    addTaskLayout->addWidget(addTaskQuesitionLabel);
    addTaskLayout->addWidget(addTaskQuesition);
    addTaskLayout->addWidget(addTaskAnswerOptionsLabel);
    addTaskLayout->addWidget(addTaskAnswerOptionsView);
    addTaskLayout->addWidget(addTaskDeleteEmpty);
    addTaskLayout->addWidget(addTaskNewOption);
    addTaskLayout->addWidget(addTaskAnswerLabel);
    addTaskLayout->addWidget(addTaskAnswer);
    addTaskLayout->addWidget(addTaskSubjectLabel);
    addTaskLayout->addWidget(addTaskSubject);
    addTaskLayout->addWidget(addTaskThemeLabel);
    addTaskLayout->addWidget(addTaskTheme);
    addTaskLayout->addWidget(addTaskSave);
    addTaskLayout->addWidget(addTaskQuit);

    setLayout(addTaskLayout);
}

void AddTaskWidget::showAddAnswerOptions()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New answer option:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        addTaskAnswerOptions.push_back(text);
        addTaskAnswerOptionsModel->setStringList(addTaskAnswerOptions);
        addTaskAnswerOptionsView->setModel(addTaskAnswerOptionsModel);
    }
}
void AddTaskWidget::addTaskAnswerOptionsDeleteEmpty()
{
    for(int i = 0; i < addTaskAnswerOptionsModel->rowCount(); ++i)
    {
        if(addTaskAnswerOptionsModel->stringList().at(i) == "")
            addTaskAnswerOptionsModel->removeRow(i);
    }
    addTaskAnswerOptionsView->setModel(addTaskAnswerOptionsModel);
}
