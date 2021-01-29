#ifndef EDITLISTBOX_H
#define EDITLISTBOX_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QDebug>
#include <QGroupBox>

class EditListBox : public QWidget
{
    Q_OBJECT
public:
    explicit EditListBox(QString name, QList <QString> l = {}, QWidget *parent = nullptr);
        bool isChanged = 0;
private:
        QGroupBox *viewBox;
        QListView *view;
        QItemSelectionModel *selection;
        QStringListModel *model;
        QList <QString> list;
        QLabel *title;
        QPushButton *add;
        QPushButton *erase;
        QPushButton *moveUp;
        QPushButton *moveDown;
        QVBoxLayout *buttonsLayout;
        QHBoxLayout *viewAndButtonsLayout;
        QVBoxLayout *layout;
private:
    void addString();
    void removeString();
    void moveStringUp();
    void moveStringDown();




signals:

};

#endif // EDITLISTBOX_H
