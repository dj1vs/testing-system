// Copyright 2021 Dmitriy Trifonov
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "AddTaskWidget.h"

AddTaskWidget::AddTaskWidget(QWidget *parent) : QWidget(parent) {
    quesitionLabel = new QLabel("Вопрос:");
    answerLabel = new QLabel("Ответ");
    answerOptionsLabel = new QLabel("Варианты ответа");
    themeLabel = new QLabel("Тема");
    subjectLabel = new QLabel("Предмет");

    save = new QPushButton("Сохранить");
    quit = new QPushButton("Выйти");
    newOption = new QPushButton("Добавить вариант ответа");
    deleteSelected = new QPushButton("Удалить выделенное");
    addImage = new QPushButton("Добавить изображение");

    connect(newOption, &QPushButton::clicked, this,
            [this] {showAddAnswerOptions(); });

    connect(deleteSelected, &QPushButton::clicked, this,
            [this] {answerOptionsDeleteSelected(); });

    connect(addImage, &QPushButton::clicked, this, [this] {
        imageFileName = QFileDialog::getOpenFileName(this,
            tr("Открыть изображение"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
        int fileSize = QFile(imageFileName).size();
        if (fileSize > 104857600) {
            QMessageBox msgBox; msgBox.setText("File is too large! (> 100 MB).\nTry to compress it or use other file."); msgBox.show();
        } else {
            imageFileName = ":NONE:";
        }
    });

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
    layout->addWidget(addImage);
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
    QString text = QInputDialog::getText(this, tr("Ввод"),
                                         tr("Новый вариант ответа:"), QLineEdit::Normal, "", &ok);
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
