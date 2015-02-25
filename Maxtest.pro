#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T20:06:18
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maxtest
TEMPLATE = app

DESTDIR += ../Testik_bin

SOURCES += main.cpp\
        maxtest.cpp \
    test_results.cpp \
    test.cpp \
    begin_first.cpp \
    third.cpp \
    second.cpp \
    admin_form.cpp \
    test_help.cpp \
    change_admin_pw_dialog.cpp \
    crypt.cpp

HEADERS  += maxtest.h \
    test_results.h \
    test.h \
    begin_first.h \
    third.h \
    second.h \
    admin_form.h \
    test_help.h \
    change_admin_pw_dialog.h \
    crypt.h

FORMS    += maxtest.ui \
    second.ui \
    testendquestion.ui \
    test_results.ui \
    test.ui \
    begin_first.ui \
    third.ui \
    admin_form.ui \
    test_help.ui \
    change_admin_pw_dialog.ui

DISTFILES += \
    muse.zip \
    maxtest.prp
