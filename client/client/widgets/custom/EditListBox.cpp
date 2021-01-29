#include "EditListBox.h"

EditListBox::EditListBox(QString name, QList <QString> l, QWidget *parent) : QWidget(parent), list(l)
{
    view = new QListView();
    view->setMovement(QListView::Free);
    model = new QStringListModel(list);
    selection = new QItemSelectionModel();
    model->setStringList(list);
    view->setModel(model);
    selection->setModel(model);
    title = new QLabel(name);
    add     = new QPushButton("+ Add");
    erase   = new QPushButton("- Erase");
    moveUp  = new QPushButton("Move Up");
    moveDown= new QPushButton("Move Down");

    connect(add, &QPushButton::clicked, this, &EditListBox::addString);
    connect(erase, &QPushButton::clicked, this, &EditListBox::removeString);
    connect(moveUp, &QPushButton::clicked, this, &EditListBox::moveStringUp);
    connect(moveDown, &QPushButton::clicked, this, &EditListBox::moveStringDown);


    buttonsLayout = new QVBoxLayout();
    buttonsLayout->addWidget(add);
    buttonsLayout->addWidget(erase);
    buttonsLayout->addWidget(moveUp);
    buttonsLayout->addWidget(moveDown);

    viewBox = new QGroupBox();

    QVBoxLayout *lay = new QVBoxLayout();
    lay->addWidget(view);
    viewBox->setLayout(lay);

    viewAndButtonsLayout = new QHBoxLayout();
    viewAndButtonsLayout->addWidget(viewBox);
    viewAndButtonsLayout->addLayout(buttonsLayout);
    layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addLayout(viewAndButtonsLayout);
    setLayout(layout);
}

void EditListBox::addString()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("User name:"), QLineEdit::Normal,
                                             "", &ok);
    if(ok && !text.isEmpty())
    {
        QStringList l = model->stringList();
        l.push_back(text);
        model->setStringList(l);
        view->setModel(model);
    }
}
void EditListBox::removeString()
{
    QModelIndex l = selection->currentIndex();
    QStringList k = model->stringList();
    int row = l.row();
    title->setText(QString::number(row));
    k.removeAt(row);
    model->setStringList(k);
    view->setModel(model);
    repaint();

}
void EditListBox::moveStringUp()
{
    QModelIndex in = selection->currentIndex();
    QStringList l = model->stringList();
    if(in.row() > 0 && in.row() < l.size())
        std::swap(l[in.row()], l[in.row()-1]);
    model->setStringList(l);
    view->setModel(model);
}
void EditListBox::moveStringDown()
{
    QModelIndex in = selection->currentIndex();
    QStringList l = model->stringList();
    if(in.row() < l.size()-1 && in.row() >= 0)
        std::swap(l[in.row()], l[in.row()+1]);
    model->setStringList(l);
    view->setModel(model);
}
