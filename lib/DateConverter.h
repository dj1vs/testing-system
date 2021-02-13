#ifndef DATECONVERTER_H
#define DATECONVERTER_H

#include <QString>
#include <QDate>
class DateConverter {
public:
    DateConverter() {};
    ~DateConverter() {};
    static QString DateToStringFormat(QDate date, QString format)
    {
        const QStringList formats = {"DD-MM-YYYY", "MM-DD-YYYY", "MM-DD-YY"};
        int day = date.day();
        int month = date.month();
        int year = date.year();
        switch(formats.indexOf(format))
        {
        case -1:
            return "unknown-format";
        case 0:
            return (day < 10 ? "0" : "") + QString::number(day) + "-" +
        (month < 10 ? "0" : "") + QString::number(month) + "-" + QString::number(year);
        case 1:
            return (month < 10 ? "0" : "") + QString::number(month) + "-" +
        (day < 10 ? "0" : "") + QString::number(day) + "-" +QString::number(year);
        case 2:
            return (month < 10 ? "0" : "") + QString::number(month) + "-" +
        (day < 10 ? "0" : "") + QString::number(day) + "-" + QString::number(year).mid(2,2);
        }
        return "";
    }
};
#endif // DATECONVERTER_H
