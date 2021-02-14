// Copyright 2021 Dmitriy Trifonov
#include "TeacherResultsWidget.h"

TeacherResultsWidget::TeacherResultsWidget(QWidget *parent, QList <QList <QString>> l) : QWidget(parent), list(l) {
    for (auto &i : list) {
        if(students.indexOf({i[0], i[1]}) == -1)
            students.push_back({i[0], i[1]});
        if(groups.indexOf(i[2]) == -1)
            groups.push_back(i[2]);
        if(testNames.indexOf(i[3]) == -1)
            testNames.push_back(i[3]);
        if(subjects.indexOf(i[5]) == -1)
            subjects.push_back(i[5]);
    }

    goBack = new QPushButton("go back");
    sort = new QPushButton("sort");
    table = new QTableView();
    model = new QStandardItemModel(list.size(), params.size());
    for (int i = 0; i < params.size(); ++i) {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for (int row = 0; row < list.size(); ++row) {
        for (int col = 0; col < params.size(); ++col) {
            QModelIndex index = model->index(row, col, QModelIndex());
            model->setData(index, list[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    connect(sort, &QPushButton::clicked, this, &TeacherResultsWidget::showSort);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(sort);
    layout->addWidget(goBack);

    setLayout(layout);
}

void TeacherResultsWidget::showSort() {
    studentBox = new QComboBox();
    studentBox->addItem("");
    for (auto &i : students)
        studentBox->addItem(i[0] + " " + i[1]);
    studentBox->setCurrentIndex(0);


    groupBox = new QComboBox();
    groupBox->addItem("");
    for (auto &i : groups)
        groupBox->addItem(i);
    groupBox->setCurrentIndex(0);


    testnameBox = new QComboBox();
    testnameBox->addItem("");
    for (auto &i : testNames)
        testnameBox->addItem(i);
    testnameBox->setCurrentIndex(0);

    subjectBox = new QComboBox();
    subjectBox->addItem("");
    for (auto &i : subjects)
        subjectBox->addItem(i);
    subjectBox->setCurrentIndex(0);

    sortByDate = new QCheckBox("Sort bu date");
    sortDateEdit = new QDateEdit();
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    QFormLayout *l = new QFormLayout();
    l->addRow(tr("student"), studentBox);
    l->addRow(tr("group"), groupBox);
    l->addRow(tr("testname"), testnameBox);
    l->addRow(tr("subject"), subjectBox);
    l->addRow(sortByDate);
    l->addRow(sortDateEdit);
    l->addRow(buttonBox);

    QDialog *d = new QDialog();
    connect(buttonBox, &QDialogButtonBox::accepted, d, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, d, &QDialog::reject);
    d->setLayout(l);
    if (d->exec() == QDialog::Accepted)
        editTable();
}

void TeacherResultsWidget::editTable() {
    QList <QString> student = studentBox->currentText().split(" ");
    QString group = groupBox->currentText();
    QString testname = testnameBox->currentText();
    QString subject = subjectBox->currentText();
    QString date = DateConverter::DateToStringFormat(sortDateEdit->date(), "DD-MM-YYYY");

    if (studentBox->currentText() == "" && group == "" && testname == "" && subject == "" && !sortByDate->isChecked()) {
        model->setRowCount(list.size());
        for (int row = 0; row < list.size(); ++row) {
            for (int col = 0; col < params.size(); ++col) {
                QModelIndex index = model->index(row, col, QModelIndex());
                model->setData(index, list[row][col]);
            }
        }
    } else {
        for (int row = 0; row < model->rowCount(); ++row) {
            if ((studentBox->currentText() != "" &&
                (model->data(model->index(row, 0, QModelIndex())).toString() != student[0] || model->data(model->index(row, 1, QModelIndex())).toString() != student[1]))
            || (group != "" && model->data(model->index(row, 2, QModelIndex())).toString() != group)
            || (testname != "" && model->data(model->index(row, 3, QModelIndex())).toString() != testname)
            || (subject != "" && model->data(model->index(row, 5, QModelIndex())).toString() != subject)
            || (sortByDate->isChecked() && date != "01-01-2000" && model->data(model->index(row, 6, QModelIndex())).toString() != date)) {
                model->removeRow(row);
                row--;
            }
        }
    }
}
