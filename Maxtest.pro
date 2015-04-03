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
    crypt.cpp \
    question_mod_dialog.cpp \
    dbfunc.cpp \
    theme_dlg.cpp \
    stud_dlg.cpp

HEADERS  += maxtest.h \
    test_results.h \
    test.h \
    begin_first.h \
    third.h \
    second.h \
    admin_form.h \
    test_help.h \
    change_admin_pw_dialog.h \
    crypt.h \
    question_mod_dialog.h \
    dbfunc.h \
    theme_dlg.h \
    stud_dlg.h

FORMS    += maxtest.ui \
    second.ui \
    testendquestion.ui \
    test_results.ui \
    test.ui \
    begin_first.ui \
    third.ui \
    admin_form.ui \
    test_help.ui \
    change_admin_pw_dialog.ui \
    question_mod_dialog.ui \
    theme_dlg.ui \
    stud_dlg.ui

DISTFILES += \
    muse.zip \
    maxtest.prp \
    data/students.csv \
    buglist.txt \
    drivers/win32/qsqlibase.dll \
    drivers/win32/qsqlibased.dll \
    drivers/mac_os_x64/libqsqlibase_debug.dylib \
    drivers/mac_os_x64/libqsqlibase.dylib \
    data/QSLITE_BLANK.QLT \
    data/db_ddl.sql

RESOURCES += \
    res.qrc
