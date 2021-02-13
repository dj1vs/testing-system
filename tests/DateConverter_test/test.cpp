#include "test.h"
#include "test.h"
#include "../../lib/DateConverter.h"
#include <QTest>

test::test(QObject *parent) : QObject(parent)
{

}

void test::DateToStringFormat()
{
    DateConverter a;
    QCOMPARE(a.DateToStringFormat(QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy"), "DD-MM-YYYY"), "01-12-2003");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("12MM12car2012", "d'MM'MMcaryyyy"), "DD-MM-YYYY"), "12-12-2012");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("20000110", "yyyyMMdd"), "DD-MM-YYYY"), "10-01-2000");
    QCOMPARE(a.DateToStringFormat( QDate::fromString("1.1", "M.d"), "DD-MM-YYYY"), "01-01-1900");

    QCOMPARE(a.DateToStringFormat(QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy"), "MM-DD-YYYY"), "12-01-2003");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("12MM12car2012", "d'MM'MMcaryyyy"), "MM-DD-YYYY"), "12-12-2012");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("20000110", "yyyyMMdd"), "MM-DD-YYYY"), "01-10-2000");
    QCOMPARE(a.DateToStringFormat( QDate::fromString("1.1", "M.d"), "MM-DD-YYYY"), "01-01-1900");

    QCOMPARE(a.DateToStringFormat(QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy"), "MM-DD-YY"), "12-01-03");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("12MM12car2012", "d'MM'MMcaryyyy"), "MM-DD-YY"), "12-12-12");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("20000110", "yyyyMMdd"), "MM-DD-YY"), "01-10-00");
    QCOMPARE(a.DateToStringFormat( QDate::fromString("1.1", "M.d"), "MM-DD-YY"), "01-01-00");

    QCOMPARE(a.DateToStringFormat(QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy"), "MM-DD-"), "unknown-format");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("12MM12car2012", "d'MM'MMcaryyyy"), "MM-D-YY"), "unknown-format");
    QCOMPARE(a.DateToStringFormat(QDate::fromString("20000110", "yyyyMMdd"), "MM-DDddd-YY"), "unknown-format");
    QCOMPARE(a.DateToStringFormat( QDate::fromString("1.1", "M.d"), "MM-Dd-YY"), "unknown-format");
}
