// Copyright 2021 Dmitriy Trifonov

#include <QTest>
#include "test.h"
#include "../../lib/StringOperator.h"
void test::cutArg() {
    StringOperator a;

    //  check correct strings
    QCOMPARE(a.cutArg("{cmd='testing';}", "cmd"), "testing");
    QCOMPARE(a.cutArg("{cmd='make break' ;fa''s't';     drink='milk'; time='9 PM';worktime='15 min';}", "drink"), "milk");
    QCOMPARE(a.cutArg("{cmd='make break' ;fa''s't';     drink='milk'; time='9 PM';worktime='15 min';}", "time"), "9 PM");

    //  check strings with ' and/or ;
    QCOMPARE(a.cutArg("{cmd='test''''''';}", "cmd"), "test''''''");
    QCOMPARE(a.cutArg("{cmd='test;;; ; ; ;    ;'';}", "cmd"), "test;;; ; ; ;    ;'");

    //  check finding not esisting arg
    QCOMPARE(a.cutArg("{cmd='do something';}", "do it"), "");

    //  check finding arg that appears in arg text for another arg
    QCOMPARE(a.cutArg("{cmd='do something';}", "do something"), "");

    //  check empty string
    QCOMPARE(a.cutArg("{}", ""), "");
}
void test::validatePackage() {
    StringOperator a;
    //  test for empty package / only brackets package
    QCOMPARE(a.validatePackage(""), 0);
    QCOMPARE(a.validatePackage("{"), 0);
    QCOMPARE(a.validatePackage("{}"), 0);

    //  check basics for no closing scope or no ;
    QCOMPARE(a.validatePackage("{cmd='}"), 0);
    QCOMPARE(a.validatePackage("{cmd=''}"), 0);

    //  check valid packages
    QCOMPARE(a.validatePackage("{cmd='testing';}"), 1);
    QCOMPARE(a.validatePackage("{cmd='make break' ;fa''s't';     drink='milk'; time='9 PM';worktime='15 min';}"), 1);

    //  check package without last ;
    QCOMPARE(a.validatePackage("{cmd='make break' ;fa''s't';     drink='milk'; time='9 PM';worktime='15 min'}"), 0);



    //  NOTE: if we will remove some ; or ' in the middle of the package,
    //  all message will just become a long text for last 'correct' arg, so this tests
    //  are not being used
    //  to avoid this situation we just need to write commands carefully and
    //  check users input
    //
    //
    //  Example (using spaces to show request parts):
    //  {cmd = 'something';
    //  arg1 = 'hi';}
    //  After removing ' or ;:
    //  {cmd = 'something'arg1 = 'hi';}
    //          ____________________ - one argument text
}

void test::validateUserInput() {
    StringOperator a;
    QCOMPARE(a.validateUserInput("ytfghirwefaghnr"), "ytfghirwefaghnr");
    QCOMPARE(a.validateUserInput("ytfghirwefa';ghnr"), "ytfghirwefa' ;ghnr");
    QCOMPARE(a.validateUserInput("='ytfghirwefaghnr"), "= 'ytfghirwefaghnr");
    QCOMPARE(a.validateUserInput("ytfghir =';wefaghnr"), "ytfghir = ' ;wefaghnr");
    QCOMPARE(a.validateUserInput(""), "");
    QCOMPARE(a.validateUserInput("=' zxczxczxc ';"), "= ' zxczxczxc ' ;");
}
