QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
LIBS += -L/usr/include/KF5/KDELibs4Support/*

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    myclient.cpp \
    widgets/Admin/AdminWidget.cpp \
    widgets/Admin/AllGroupsWidget.cpp \
    widgets/Admin/AllResultsWidget.cpp \
    widgets/Admin/AllTestsWidget.cpp \
    widgets/AdminPlus/AddGroupWidget.cpp \
    widgets/AdminPlus/AddToGroupWidget.cpp \
    widgets/AdminPlus/AddUserWidget.cpp \
    widgets/AdminPlus/AdminPlusWidget.cpp \
    widgets/AdminPlus/AppointGroupWidget.cpp \
    widgets/AuthorizationWidget.cpp \
    widgets/Teacher/AddTaskWidget.cpp \
    widgets/Teacher/AddTestWidget.cpp \
    widgets/Teacher/TeacherGroupsWidget.cpp \
    widgets/Teacher/TeacherResultsWidget.cpp \
    widgets/Teacher/TeacherWidget.cpp \
    widgets/custom/EditListBox.cpp

HEADERS += \
    lib/DateConverter.h \
    myclient.h \
    widgets/Admin/AdminWidget.h \
    widgets/Admin/AllGroupsWidget.h \
    widgets/Admin/AllResultsWidget.h \
    widgets/Admin/AllTestsWidget.h \
    widgets/AdminPlus/AddGroupWidget.h \
    widgets/AdminPlus/AddToGroupWidget.h \
    widgets/AdminPlus/AddUserWidget.h \
    widgets/AdminPlus/AdminPlusWidget.h \
    widgets/AdminPlus/AppointGroupWidget.h \
    widgets/AuthorizationWidget.h \
    widgets/Teacher/AddTaskWidget.h \
    widgets/Teacher/AddTestWidget.h \
    widgets/Teacher/TeacherGroupsWidget.h \
    widgets/Teacher/TeacherResultsWidget.h \
    widgets/Teacher/TeacherWidget.h \
    widgets/custom/EditListBox.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
