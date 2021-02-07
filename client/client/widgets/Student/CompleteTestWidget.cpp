#include "CompleteTestWidget.h"

CompleteTestWidget::CompleteTestWidget(QList <QList <QString>> list, QWidget *parent) : QWidget(parent), testList(list)
{
    qDebug() << testList;
    testname = testList[0][0];
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
    QMessageBox msgBox;
    msgBox.setText("Вы собираетесь закончить тест!");
    msgBox.setInformativeText("Ок - закончить тест\nCancel - продолжить выполнение");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (msgBox.exec() != QMessageBox::Ok)
        return;
    int answerSum = 0;
    for (int i = 0; i < answers.size(); ++i)
    {
        if (answers[i] == testList[i][3])
        {
            isAnswersCorrect.push_back(1);
            answerSum++;
        }
        else
           isAnswersCorrect.push_back(0);
    }

    percent = QString::number((int)(((answerSum * 1.0) / (answers.size() * 1.0)) * 100.0));

    QTableView *table = new QTableView();
    QStandardItemModel *model = new QStandardItemModel(answers.size(), 2, nullptr);

    const QStringList params = {"Task number", "Status"};

    for(int i = 0; i < params.size(); ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < answers.size(); ++row)
    {
        QModelIndex index=model->index(row,0,QModelIndex());
        model->setData(index, row+1);
        index=model->index(row,1,QModelIndex());
        model->setData(index, (isAnswersCorrect[row] ? "correct" : "wrong"));
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    QLabel *title = new QLabel("You completed the test\nResult:" + percent + "percents");

    QVBoxLayout *lay = new QVBoxLayout();
    lay->addWidget(title);
    lay->addWidget(table);

    QDialog *d = new QDialog();
    d->setLayout(lay);
    d->exec();

    emit finished();




}
