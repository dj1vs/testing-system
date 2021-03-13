#ifndef IMAGEVIEWDIALOG_H
#define IMAGEVIEWDIALOG_H

#include <QDialog>
#include <QFile>
#include <QString>
#include <QImageReader>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>

class ImageViewDialog {
public:
    ImageViewDialog();
    ~ImageViewDialog();
    static void ViewImage(QString fileName) {
        QImageReader reader(fileName);
        const QImage newImage = reader.read();
         if (newImage.isNull()) {
             QMessageBox msg; msg.setText("Cant load image");
             msg.show();
             return;
         }
         QPixmap image(fileName);
         QDialog *d = new QDialog();
         d->setMinimumSize(100, 100);
         QLabel *imageLabel = new QLabel();
         imageLabel->setPixmap(image);
         imageLabel->setScaledContents(1);
         QVBoxLayout *l = new QVBoxLayout();
         l->addWidget(imageLabel);

         d->setLayout(l);
         d->show();




    }
};

#endif // IMAGEVIEWDIALOG_H
