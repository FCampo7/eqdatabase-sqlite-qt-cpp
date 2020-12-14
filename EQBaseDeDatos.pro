#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T17:30:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EQBaseDeDatos
TEMPLATE = app


SOURCES += main.cpp\
        eqbasedatos.cpp \
    agregarp.cpp \
    producto.cpp \
    userlogin.cpp \
    admin.cpp

HEADERS  += eqbasedatos.h \
    agregarp.h \
    producto.h \
    userlogin.h \
    admin.h

FORMS    += eqbasedatos.ui \
    agregarp.ui \
    userlogin.ui \
    admin.ui

QT      += sql

RESOURCES += \
    source.qrc

RC_FILE += eqbd.rc
