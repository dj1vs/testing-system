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
    setManualTaskList({{"1", "2", "3", "4", "5", "6", "7"}});
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

    for(int i = 0; i < 5; ++i)
    {
        QPushButton *buf = new QPushButton(manualButtonsTexts[i]);
        manualButtons.push_back(buf);
    }

//    const QList <QString> manualButtonsTexts =
//    {"Sort", "Pick","Delete","Move Up","Move Down"};

    connect(manualButtons[1], &QPushButton::clicked, this, [this]
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

    connect(manualButtons[2], &QPushButton::clicked, this, [this] {
       if(manualPicked->selectionModel()->hasSelection())
       {
           int row = manualPicked->selectionModel()->currentIndex().row();
           manualPickedModel->removeRow(row);
       }
    });

    connect(manualButtons[3], &QPushButton::clicked, this, [this]
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

    manualButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

    manualLayout = new QVBoxLayout();
    QVBoxLayout *allButtons = new QVBoxLayout();
    allButtons->addWidget(manualButtons[0]);
    allButtons->addWidget(manualButtons[1]);
    QVBoxLayout *allTable = new QVBoxLayout();
    allTable->addWidget(manualAllLabel);
    allTable->addWidget(manualAll);
    QHBoxLayout *allLayout = new QHBoxLayout();
    allLayout->addLayout(allTable);
    allLayout->addLayout(allButtons);

    QVBoxLayout *pickedButtons = new QVBoxLayout();
    allButtons->addWidget(manualButtons[2]);
    allButtons->addWidget(manualButtons[3]);
    allButtons->addWidget(manualButtons[4]);
    QVBoxLayout *pickedTable = new QVBoxLayout();
    allTable->addWidget(manualPickedLabel);
    allTable->addWidget(manualPicked);
    QHBoxLayout *pickedLayout = new QHBoxLayout();
    allLayout->addLayout(pickedTable);
    allLayout->addLayout(pickedButtons);

    manualLayout->addLayout(allLayout);
    manualLayout->addLayout(pickedLayout);
    manualLayout->addWidget(manualButtonBox);

    QDialog *d = new QDialog();
    connect(manualButtonBox, &QDialogButtonBox::accepted, d, &QDialog::accept);
    connect(manualButtonBox, &QDialogButtonBox::rejected, d, &QDialog::reject);
    d->setLayout(manualLayout);
    if(d->exec() == QDialog::Accepted)
        emit finished();
}
