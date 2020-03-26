#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T03:32:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = revisorV4
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += main.cpp\
        mainwindow.cpp \
    cars_list.cpp \
    menu.cpp \
    sklad.cpp \
    zakaznaryad.cpp \
    userinfo.cpp \
    globals.cpp \
    qjsontable.cpp \
    adduser.cpp \
    edituser.cpp \
    settyre.cpp \
    addgsm.cpp \
    tyreforcar.cpp \
    opendopogfile.cpp \
    alarm.cpp

HEADERS  += mainwindow.h \
    cars_list.h \
    menu.h \
    sklad.h \
    zakaznaryad.h \
    userinfo.h \
    globals.h \
    qjsontable.h \
    adduser.h \
    edituser.h \
    settyre.h \
    addgsm.h \
    tyreforcar.h \
    opendopogfile.h \
    alarm.h


FORMS    += mainwindow.ui \
    cars_list.ui \
    menu.ui \
    menu.ui \
    sklad.ui \
    zakaznaryad.ui \
    userinfo.ui \
    adduser.ui \
    edituser.ui \
    settyre.ui \
    addgsm.ui \
    tyreforcar.ui \
    opendopogfile.ui


RESOURCES += \
    resource.qrc


