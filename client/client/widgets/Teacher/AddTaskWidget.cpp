// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "AddTaskWidget.h"

AddTaskWidget::AddTaskWidget(QWidget *parent) : QWidget(parent) {
    quesitionLabel = new QLabel("Вопрос:");
    answerLabel = new QLabel("Answer");
    answerOptionsLabel = new QLabel("Answer options");
    themeLabel = new QLabel("Theme");
    subjectLabel = new QLabel("Subject");

    save = new QPushButton("save");
    quit = new QPushButton("quit");
    newOption = new QPushButton("add new answer option");
    deleteSelected = new QPushButton("delete selected");

    connect(newOption, &QPushButton::clicked, this,
            [this] {showAddAnswerOptions(); });

    connect(deleteSelected, &QPushButton::clicked, this,
            [this] {answerOptionsDeleteSelected(); });

    quesition = new QTextEdit();
    answer = new QTextEdit();

    answerOptionsModel = new QStringListModel();
    answerOptionsView = new QListView();

    theme = new QLineEdit();
    subject = new QLineEdit();

    QVBoxLayout *answerOptionsButtonsLayout = new QVBoxLayout();
    answerOptionsButtonsLayout->addWidget(deleteSelected);
    answerOptionsButtonsLayout->addWidget(newOption);

    QHBoxLayout *answerOptionsLayout = new QHBoxLayout();
    answerOptionsLayout->addWidget(answerOptionsView);
    answerOptionsLayout->addLayout(answerOptionsButtonsLayout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(quesitionLabel);
    layout->addWidget(quesition);
    layout->addWidget(answerOptionsLabel);
    layout->addLayout(answerOptionsLayout);
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

void AddTaskWidget::showAddAnswerOptions() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New answer option:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        answerOptions.push_back(text);
        answerOptionsModel->setStringList(answerOptions);
        answerOptionsView->setModel(answerOptionsModel);
    }
}
void AddTaskWidget::answerOptionsDeleteSelected() {
    int row = answerOptionsView->selectionModel()->currentIndex().row();
    answerOptionsModel->removeRow(row);
    answerOptions.removeAt(row);
    answerOptionsView->setModel(answerOptionsModel);
}
