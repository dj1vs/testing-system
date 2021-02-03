#include "CompleteTestWidget.h"

CompleteTestWidget::CompleteTestWidget(QList <QList <QString>> list, QWidget *parent) : QWidget(parent), testList(list)
{
    for(int i = 0; i < testList.size(); ++i)
        answers.push_back("");
    qDebug() << testList;
    currentIndex = 0;
    next = new QPushButton("next");
    previous = new QPushButton("prev");
    taskText = new QTextBrowser();
    buttonsBox = new QGroupBox();

    connect(next, &QPushButton::clicked, this, [this] {
        for(int i = 0; i < answerOptionButtons.size(); ++i)
        {
            if(answerOptionButtons[i]->isChecked())
            {
                answers[currentIndex] = answerOptionButtons[i]->text();
                break;
            }
        }
        if (currentIndex+1 == testList.size())
            askIfFinished();
        else
        {
            currentIndex++;
            setIndexTask();
        }
    });

    connect(previous, &QPushButton::clicked, this, [this] {
        for(int i = 0; i < answerOptionButtons.size(); ++i)
        {
            if(answerOptionButtons[i]->isChecked())
            {
                answers[currentIndex] = answerOptionButtons[i]->text();
                break;
            }
        }
        if (currentIndex)
        {
            currentIndex--;
            setIndexTask();
        }
    });

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(previous);
    buttonsLayout->addWidget(next);

    generalLayout = new QVBoxLayout();
    generalLayout->addWidget(taskText);
    generalLayout->addWidget(buttonsBox);
    generalLayout->addLayout(buttonsLayout);

    setLayout(generalLayout);

    setIndexTask();
}

void CompleteTestWidget::setIndexTask()
{
    generalLayout->removeItem(generalLayout->itemAt(1));
    delete buttonsBox;
    buttonsBox = new QGroupBox();
    answerOptionButtons.clear();
    taskText->setText(testList[currentIndex][1]);
    QStringList answerOptions = testList[currentIndex][2].split(";");
    QVBoxLayout *l = new QVBoxLayout();
    for(int i = 0; i < answerOptions.size(); ++i)
    {
        if(answerOptions[i] == "")
            continue;
        QRadioButton *buf = new QRadioButton(answerOptions[i], this);
        if(answerOptions[i] == answers[currentIndex])
            buf->setChecked(1);
        l->addWidget(buf);
        answerOptionButtons.push_back(buf);
    }
    buttonsBox->setLayout(l);
    generalLayout->insertWidget(1, buttonsBox);
}

void CompleteTestWidget::askIfFinished()
{
    qDebug() << answers;
}
