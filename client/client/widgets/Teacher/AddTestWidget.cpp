#include "AddTestWidget.h"

AddTestWidget::AddTestWidget(QWidget *parent) : QWidget(parent)
{
    QList <QLabel *> labels;
    const QList <QString> labelNames = {"Test name", "Subject", "Date"};
    for(int i = 0; i < labelNames.size(); ++i)
    {
        QLabel *buf = new QLabel(labelNames[i], this);
        labels.push_back(buf);
    }
    nameEdit = new QLineEdit(this);
    subjectEdit = new QLineEdit(this);
    isManualCheck = new QCheckBox("Add task manual?", this);
    createButton = new QPushButton("Create test", this);
    dateEdit = new QCalendarWidget(this);
    dateEdit->setMinimumDate(QDate::currentDate());
    connect(createButton, &QPushButton::clicked, this, [this]
    {
        if(checkStart())
        {
            if(!isManualCheck->isChecked())
            {
                state = RANDOM;
                setParamsRandom();
            }
            else
            {
                state = MANUAL;
                setManual();
            }
        }
    });

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(labels[0]);
    textLayout->addWidget(nameEdit);
    textLayout->addWidget(labels[1]);
    textLayout->addWidget(subjectEdit);
    textLayout->addWidget(isManualCheck);

    QVBoxLayout *dateLayout = new QVBoxLayout();
    dateLayout->addWidget(labels[2]);
    dateLayout->addWidget(dateEdit);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addLayout(textLayout);
    topLayout->addLayout(dateLayout);

    layout = new QVBoxLayout();
    layout->addLayout(topLayout);
    layout->addWidget(createButton);

    setLayout(layout);


}
void AddTestWidget::setParamsRandom()
{
    randomThemeLabel  = new QLabel("Theme", this);
    randomAmountLabel = new QLabel("Amount", this);
    randomThemeEdit = new QLineEdit(this);
    randomShowMy = new QCheckBox("Select only my tasks", this);
    randomAmountBox = new QSpinBox(this);
    randomAmountBox->setMinimum(1);
    randomButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

    QHBoxLayout *amountLayout = new QHBoxLayout();
    amountLayout->addWidget(randomAmountLabel);
    amountLayout->addWidget(randomAmountBox);


    randomLayout = new QVBoxLayout();
    randomLayout->addWidget(randomThemeLabel);
    randomLayout->addWidget(randomThemeEdit);
    randomLayout->addWidget(randomShowMy);
    randomLayout->addLayout(amountLayout);
    randomLayout->addWidget(randomButtonBox);

    QDialog *d = new QDialog();
    connect(randomButtonBox, &QDialogButtonBox::accepted, d, &QDialog::accept);
    connect(randomButtonBox, &QDialogButtonBox::rejected, d, &QDialog::reject);
    d->setLayout(randomLayout);
    if(d->exec() == QDialog::Accepted)
        emit finished();
}

bool AddTestWidget::checkStart()
{
    return QList <QString> {nameEdit->text(), subjectEdit->text()} != QList <QString> {"", ""};
}

void AddTestWidget::setManual()
{
    setManualTaskList({{"1", "2", "3", "4", "5", "6", "2"},
                      {"q", "w", "e", "r", "t", "y", "2"},
                      {"a", "s", "d", "f", "g", "h", "3"},
                      {"1", "2", "3", "4", "5", "6", "2"}});

    for(auto &i : manualTaskList)
    {
        manualSubjects.insert(i[1]);
        manualThemes.insert(i[5]);
    }

    manualSortTask = new QLineEdit();
    manualSortSubject = new QComboBox();
    manualSortSubject->addItem("");
    for(auto &i : manualSubjects)
        manualSortSubject->addItem(i);
    manualSortSubject->setCurrentIndex(0);
    manualSortTheme = new QComboBox();
    manualSortTheme->addItem("");
    for(auto &i : manualThemes)
        manualSortTheme->addItem(i);
    manualSortTheme->setCurrentIndex(0);
    manualSortMine = new QCheckBox("Only mine");


    manualAllLabel = new QLabel("all");
    manualPickedLabel = new QLabel("picked");
    manualAll = new QTableView();
    manualPicked = new QTableView();
    manualAllModel = new QStandardItemModel(manualTaskList.size(), 6);
    QList <QString> params = {"ID", "Subject", "Task", "Answer options", "Answer", "Theme"};
    for(int i = 0; i < 6; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        manualAllModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < manualTaskList.size(); ++row)
    {
        for(int col = 0; col < 6; ++col)
        {
            QModelIndex index=manualAllModel->index(row,col,QModelIndex());
            manualAllModel->setData(index, manualTaskList[row][col]);
        }
    }
    manualAll->setEditTriggers(QAbstractItemView::NoEditTriggers);
    manualAll->setModel(manualAllModel);

    manualPickedModel = new QStandardItemModel(0, 2);
    manualPicked->setModel(manualPickedModel);

    QList <QString> paramsPicked = {"ID", "Task"};
    for(int i = 0; i < 2; ++i)
    {
        QByteArray ba = paramsPicked[i].toLocal8Bit();
        const char* c_str = ba.data();
        manualPickedModel->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    manualSort = new QPushButton("Sort");
    manualDelete = new QPushButton("Delete");
    manualPick = new QPushButton("Pick");
    manualUp = new QPushButton("Up");
    manualDown = new QPushButton("Down");
//    const QList <QString> manualButtonsTexts =
//    {"Sort", "Pick","Delete","Move Up","Move Down"};


    connect(manualPick, &QPushButton::clicked, this, [this]
    {
        if (manualAll->selectionModel()->hasSelection())
        {
            int row = manualAll->selectionModel()->currentIndex().row();
            QList<QStandardItem *> items;
            items.append(new QStandardItem(manualTaskList[row][0]));
            items.append(new QStandardItem(manualTaskList[row][2]));
            manualPickedModel->appendRow(items);
        }
    });

    connect(manualDelete, &QPushButton::clicked, this, [this] {
       if(manualPicked->selectionModel()->hasSelection())
       {
           int row = manualPicked->selectionModel()->currentIndex().row();
           manualPickedModel->removeRow(row);
       }
    });

    connect(manualUp, &QPushButton::clicked, this, [this]
    {
        if(manualPicked->selectionModel()->hasSelection())
        {
            int row = manualPicked->selectionModel()->currentIndex().row();
            if(row)
            {
                QList <QStandardItem *> list = manualPickedModel->takeRow(row);
                manualPickedModel->insertRow(row-1, list);
            }
        }
    });
    connect(manualDown, &QPushButton::clicked, this, [this]
    {
        if(manualPicked->selectionModel()->hasSelection())
        {
            int row = manualPicked->selectionModel()->currentIndex().row();
            if(row < manualPickedModel->rowCount()-1)
            {
                QList <QStandardItem *> list = manualPickedModel->takeRow(row);
                manualPickedModel->insertRow(row+1, list);
            }
        }
    });

//        QList <QString> params = {"ID", "Subject", "Task", "Answer options", "Answer", "Theme"};

    connect(manualSort, &QPushButton::clicked, this, [this]
    {
       if(manualSortSubject->currentText() == "" && manualSortTheme->currentText() == "" &&
       !manualSortMine->isChecked() && manualSortTask->text() == "")
       {
           manualAllModel->setRowCount(manualTaskList.size());
           for(int row = 0; row < manualTaskList.size(); ++row)
           {
               for(int col = 0; col < 6; ++col)
               {
                   QModelIndex index=manualAllModel->index(row,col,QModelIndex());
                   manualAllModel->setData(index, manualTaskList[row][col]);
               }
           }
       }
       else
       {
           for(int row = 0; row < manualAllModel->rowCount(); ++row)
           {
               if((manualSortSubject->currentText() != "" && manualAllModel->data(manualAllModel->index(row,1,QModelIndex())).toString() != manualSortSubject->currentText())
                || (manualSortTheme->currentText() != "" && manualAllModel->data(manualAllModel->index(row,5,QModelIndex())).toString() != manualSortTheme->currentText())
                || (manualSortTask->text() != "" && manualAllModel->data(manualAllModel->index(row,2,QModelIndex())).toString() != manualSortTask->text()))
               {
                   manualAllModel->removeRow(row);
                   row--;
               }
               if(!manualSortMine->isChecked())
                   continue;
               for(auto &i : manualTaskList)
               {
                   qDebug() << userID << i[6] << manualAllModel->data(manualAllModel->index(row,0,QModelIndex())).toString();
                   if(i[0] == manualAllModel->data(manualAllModel->index(row,0,QModelIndex())).toString())
                   {
                       if(i[6].toInt() != userID)
                       {
                           manualAllModel->removeRow(row);
                           row--;
                       }
                       break;
                   }
               }
           }
       }
    });

    manualButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

    QFormLayout *sortLayout = new QFormLayout();
    sortLayout->addRow(tr("task text"),manualSortTask);
    sortLayout->addRow(tr("subject"),manualSortSubject);
    sortLayout->addRow(tr("theme"),manualSortTheme);
    sortLayout->addWidget(manualSortMine);
    sortLayout->addWidget(manualSort);

    QGroupBox *sortBox = new QGroupBox();
    sortBox->setLayout(sortLayout);
    sortBox->setMaximumWidth(200);
    sortBox->setMaximumHeight(300);


    QVBoxLayout *trLayout = new QVBoxLayout();
    trLayout->addWidget(manualPick);
    trLayout->addWidget(sortBox);

    QVBoxLayout *allTable = new QVBoxLayout();
    allTable->addWidget(manualAllLabel);
    allTable->addWidget(manualAll);
    QHBoxLayout *allLayout = new QHBoxLayout();
    allLayout->addLayout(allTable);
    allLayout->addLayout(trLayout);
    QGroupBox *allBox = new QGroupBox();
    allBox->setLayout(allLayout);

    QVBoxLayout *pickedButtons = new QVBoxLayout();
    pickedButtons->addWidget(manualDelete);
    pickedButtons->addWidget(manualUp);
    pickedButtons->addWidget(manualDown);

    QVBoxLayout *pickedTable = new QVBoxLayout();
    pickedTable->addWidget(manualPickedLabel);
    pickedTable->addWidget(manualPicked);

    QHBoxLayout *pickedLayout = new QHBoxLayout();
    pickedLayout->addLayout(pickedTable);
    pickedLayout->addLayout(pickedButtons);

    manualLayout = new QVBoxLayout();
    manualLayout->addWidget(allBox);
    manualLayout->addLayout(pickedLayout);
    manualLayout->addWidget(manualButtonBox);

    QDialog *d = new QDialog();
    d->setMinimumWidth(1000);
    connect(manualButtonBox, &QDialogButtonBox::accepted, d, &QDialog::accept);
    connect(manualButtonBox, &QDialogButtonBox::rejected, d, &QDialog::reject);
    d->setLayout(manualLayout);
    if(d->exec() == QDialog::Accepted)
        emit finished();
}






