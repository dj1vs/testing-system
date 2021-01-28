#include "AllTestsWidget.h"

AllTestsWidget::AllTestsWidget(QWidget *parent, QList<QList <QString>> l) : QWidget(parent), testsList(l)
{
    table = new QTableView(this);
    table->setAttribute(Qt::WA_DeleteOnClose);
    model = new QStandardItemModel(testsList.size(), testsList[0].size() + 1, this);
    QList <QString> params = {"Teacher name", "Teacher surname", "Test", "Subject", "Date", "Tasks"};
    for(int i = 0; i < testsList[0].size()+1; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < testsList.size(); ++row)
    {
        for(int col = 0; col < testsList[0].size(); ++col)
        {
            QModelIndex index=model->index(row,col,QModelIndex());
            model->setData(index, testsList[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    for(int row = 0; row < testsList.size(); ++row)
    {
        QModelIndex index=model->index(row,testsList[0].size(),QModelIndex());
        QPushButton *button = new QPushButton(this);
        list.push_back(button);
        button->setAccessibleName(model->index(row,2,QModelIndex()).data().toString());
        buttons.push_back(button);
        table->setIndexWidget(index, button);
    }

    goBack = new QPushButton("Go back");
    sort = new QPushButton("Sort");
    connect(sort, &QPushButton::clicked, this, &AllTestsWidget::showAllPlannedTestsSort);
    goBack->setAttribute(Qt::WA_DeleteOnClose);

    connect(sort, SIGNAL(clicked()), this, SLOT(showAllPlannedTestsSort()));
    layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(sort);
    layout->addWidget(goBack);

    setLayout(layout);
}
void AllTestsWidget::showAllPlannedTestsSort()
{
    sortNameLabel = new QLabel("Teacher name:", this);
    sortSurnameLabel = new QLabel("Teacher surname:", this);
    sortTestLabel = new QLabel("Test name:", this);
    sortDateLabel = new QLabel("Date (dd.MM.yyyy):", this);
    sortSubjectLabel = new QLabel("Subject:", this);
    sortName = new QLineEdit();
    sortSurname = new QLineEdit();
    sortTest = new QLineEdit();
    sortDate = new QDateEdit();
    sortSubject = new QLineEdit();
    sortViewPast = new QRadioButton("Прошедшие");
    sortViewFuture= new QRadioButton("Запланированные");
    sortViewAll= new QRadioButton("Все");
    sortSave = new QPushButton("Сохранить ");

    QHBoxLayout *name = new QHBoxLayout();
    name->addWidget(sortNameLabel);
    name->addWidget(sortName);

    QHBoxLayout *surname = new QHBoxLayout();
    surname->addWidget(sortSurnameLabel);
    surname->addWidget(sortSurname);

    QHBoxLayout *test = new QHBoxLayout();
    test->addWidget(sortTestLabel);
    test->addWidget(sortTest);

    QHBoxLayout *date = new QHBoxLayout();
    date->addWidget(sortDateLabel);
    date->addWidget(sortDate);

    QHBoxLayout *subject = new QHBoxLayout();
    subject->addWidget(sortSubjectLabel);
    subject->addWidget(sortSubject);


    QHBoxLayout *options = new QHBoxLayout();
    options->addWidget(sortViewPast);
    options->addWidget(sortViewFuture);
    options->addWidget(sortViewAll);


    sortLayout = new QVBoxLayout();
    sortLayout->addLayout(name);
    sortLayout->addLayout(surname);
    sortLayout->addLayout(test);
    sortLayout->addLayout(date);
    sortLayout->addLayout(subject);
    sortLayout->addLayout(options);
    sortLayout->addWidget(sortSave);

    QDialog *d = new QDialog();
    connect(sortSave, &QPushButton::clicked, this, [this, d] {editAllPlannedTestsTable(); d->close();});

    d->setLayout(sortLayout);
    d->show();
}
void AllTestsWidget::editAllPlannedTestsTable()
{
    QString name = sortName->text();
    QString surname = sortSurname->text();
    QString test = sortTest->text();
    QDate d = sortDate->date();
    QString date = QString::number(d.year()) + (d.month() < 10 ? "-0" : "-") + QString::number(d.month()) + (d.day() < 10 ? "-0" : "-") + QString::number(d.day());
    QString subject = sortSubject->text();
    bool isPast = sortViewPast->isChecked();
    bool isFuture = sortViewFuture->isChecked();
    bool isAll = sortViewAll->isChecked();

    if(name == "" && surname == "" && subject == "" && test == "" && date == "2000-01-01" && !isPast  && !isFuture && !isAll)
        return;

    for(int row = 0; row < model->rowCount(); ++row)
    {
        QString modelName = model->index(row,0,QModelIndex()).data().toString();
        QString modelSurname = model->index(row,1,QModelIndex()).data().toString();
        QString modelSubject = model->index(row,3,QModelIndex()).data().toString();
        QString modelTest = model->index(row,2,QModelIndex()).data().toString();
        QString modelDate = model->index(row,4,QModelIndex()).data().toString();

        if ( (name != "" &&  surname != "" && (modelName != name || modelSurname != surname)) || (test != "" && modelTest != test)
             || (date != "2000-01-01" && date != modelDate) || (subject != "" && subject != modelSubject) ||
             (isPast && QDate::fromString(modelDate) >= QDate::currentDate()) || (isFuture && QDate::fromString(modelDate) < QDate::currentDate()))
        {
            model->removeRow(row);
            row--;
        }

    }
    table->setModel(model);
}
void AllTestsWidget::showTestTasks(QList <QList<QString>> l)
{
    taskList = l;
    if (!taskList.size())
    {
        QErrorMessage *d = new QErrorMessage(this);
        d->showMessage("This test is empty");
        return;
    }
    tasksAmount = taskList.size();
    currTask = 0;
    taskNext = new QPushButton("next");
    taskPrev = new QPushButton("prev");

    connect(taskNext, SIGNAL(clicked()), this, SLOT(showNextTask()));
    connect(taskPrev, SIGNAL(clicked()), this, SLOT(showPrevTask()));
    taskText = new QTextBrowser();
    taskAnswer = new QTextBrowser();
    taskText->setText(taskList[0][1]);
    taskAnswer->setText(taskList[0][3]);
    taskAnswerOptionsModel = new QStringListModel();
    taskAnswerOptionsModel->setStringList(taskList[0][2].split(';'));
    taskAnswerOptionsView = new QListView();
    taskAnswerOptionsView->setModel(taskAnswerOptionsModel);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(taskPrev);
    buttons->addWidget(taskNext);


    taskLayout = new QVBoxLayout();
    taskLayout->addWidget(taskText);
    taskLayout->addWidget(taskAnswerOptionsView);
    taskLayout->addWidget(taskAnswer);
    taskLayout->addLayout(buttons);

    QDialog *d = new QDialog(this);
    d->setLayout(taskLayout);
    d->show();
}
void AllTestsWidget::showNextTask()
{
    if(currTask+1 >= taskList.size())
        return;
    ++currTask;
    taskText->setText(taskList[currTask][1]);
    taskAnswer->setText(taskList[currTask][3]);
    taskAnswerOptionsModel->setStringList(taskList[currTask][2].split(';'));
    taskAnswerOptionsView->setModel(taskAnswerOptionsModel);
}
void AllTestsWidget::showPrevTask()
{
    if(!currTask)
        return;
    --currTask;
    taskText->setText(taskList[currTask][1]);
    taskAnswer->setText(taskList[currTask][3]);
    taskAnswerOptionsModel->setStringList(taskList[currTask][2].split(';'));
    taskAnswerOptionsView->setModel(taskAnswerOptionsModel);
}
