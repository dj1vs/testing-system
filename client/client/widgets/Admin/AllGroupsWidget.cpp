#include "AllGroupsWidget.h"

AllGroupsWidget::AllGroupsWidget(QWidget *parent, QList <QString> l) : QWidget(parent), list(l)
{
    isTeachersOpened = false;
    isStudentsOpened = false;
    goBack = new QPushButton("Go Back", this);

    table = new QTableView(this);
    model = new QStandardItemModel(list.size(), 3, this);

    QList <QString> params = {"Name", "Teachers", "Students"};

    for(int i = 0; i < 3; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }

    for(int row = 0; row < list.size(); ++row)
    {
        QModelIndex index=model->index(row,0,QModelIndex());
        model->setData(index, list[row]);
    }

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);

    for(int row = 0; row < list.size(); ++row)
    {
        QModelIndex index1=model->index(row,1,QModelIndex());
        QString str = model->index(row,0,QModelIndex()).data().toString();
        QPushButton *button1 = new QPushButton(this);
        list1.push_back(button1);
        button1->setAccessibleName(str);

        table->setIndexWidget(index1, button1);

        QModelIndex index2=model->index(row,2,QModelIndex());
        QPushButton *button2 = new QPushButton(this);
        list2.push_back(button2);
        button2->setAccessibleName(str);

        table->setIndexWidget(index2, button2);
    }

    layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(goBack);

    setLayout(layout);
}

void AllGroupsWidget::showGroupTeachers(QList <QList <QString>> groupTeachers)
{
    if(isTeachersOpened)
        return;
    else
        isTeachersOpened = true;
    QTableView *table = new QTableView(this);
    table->setAttribute( Qt::WA_DeleteOnClose );
    QStandardItemModel *model = new QStandardItemModel(groupTeachers.size(), 2, this);
    QList <QString> params = {"Name", "Surname"};
    for(int i = 0; i < 2; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < groupTeachers.size(); ++row)
    {
        for(int col = 0; col < 2; ++col)
        {
            QModelIndex index=model->index(row,col,QModelIndex());
            model->setData(index, groupTeachers[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    QDialog *d = new QDialog(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(table);
    d->setLayout(v);
    d->exec();
    isTeachersOpened = false;
}
void AllGroupsWidget::showGroupStudents(QList <QList <QString>> groupStudents)
{
    if(isStudentsOpened)
        return;
    else
        isStudentsOpened = true;
    QTableView *table = new QTableView(this);
    table->setAttribute( Qt::WA_DeleteOnClose );
    QStandardItemModel *model = new QStandardItemModel(groupStudents.size(), 2, this);
    QList <QString> params = {"Name", "Surname"};
    for(int i = 0; i < 2; ++i)
    {
        QByteArray ba = params[i].toLocal8Bit();
        const char* c_str = ba.data();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(c_str));
    }
    for(int row = 0; row < groupStudents.size(); ++row)
    {
        for(int col = 0; col < 2; ++col)
        {
            QModelIndex index=model->index(row,col,QModelIndex());
            model->setData(index, groupStudents[row][col]);
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setModel(model);
    QDialog *d = new QDialog(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(table);
    d->setLayout(v);
    d->exec();
    isStudentsOpened = false;
}
