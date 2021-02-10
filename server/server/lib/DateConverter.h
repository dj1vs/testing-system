#ifndef DATECONVERTER_H
#define DATECONVERTER_H

#include <QString>
#include <QDate>
class DateConverter {
public:
    DateConverter() {};
    ~DateConverter() {};
    static QString DateToStringFromat(QDate date, QString format)
    {
        int day = date.day();
        int month = date.month();
        int year = date.year();
        if(format == "DD-MM-YYYY")
            return (day < 10 ? "0" : "") + QString::number(day) + "-" +
        (month < 10 ? "0" : "") + QString::number(month) + "-" + QString::number(year);
        return "";
    }
};
#endif // DATECONVERTER_H
