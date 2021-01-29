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
  //  isManualCheck = new QCheckBox("Add task manual?", this);
    createButton = new QPushButton("Create test", this);
    dateEdit = new QCalendarWidget(this);
    dateEdit->setMinimumDate(QDate::currentDate());
    connect(createButton, &QPushButton::clicked, this, [this]
    {
        if(checkStart())
            setParamsRandom();
    });

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(labels[0]);
    textLayout->addWidget(nameEdit);
    textLayout->addWidget(labels[1]);
    textLayout->addWidget(subjectEdit);
 //   textLayout->addWidget(isManualCheck);

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
