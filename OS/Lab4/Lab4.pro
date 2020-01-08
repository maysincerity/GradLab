#-------------------------------------------------
#
# Project created by QtCreator 2019-03-08T14:37:16
#
#-------------------------------------------------

QT       += core gui
RC_ICONS += 1.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Lab4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    procdlg.cpp \
    showcurve.cpp \
    getcurve.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    datastructure.h \
    procdlg.h \
    showcurve.h \
    getcurve.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    procdlg.ui
