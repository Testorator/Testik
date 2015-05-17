#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T20:06:18
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maxtest
TEMPLATE = app

DESTDIR += ../Testik_bin

SOURCES += main.cpp\
        maxtest.cpp \
    test_results.cpp \
    test.cpp \
    third.cpp \
    second.cpp \
    admin_form.cpp \
    test_help.cpp \
    change_admin_pw_dialog.cpp \
    crypt.cpp \
    question_mod_dialog.cpp \
    dbfunc.cpp \
    theme_dlg.cpp \
    stud_dlg.cpp \
    encryption.cpp \
    email.cpp \
    email_dlg.cpp \
    smtpclient/emailaddress.cpp \
    smtpclient/mimeattachment.cpp \
    smtpclient/mimecontentformatter.cpp \
    smtpclient/mimefile.cpp \
    smtpclient/mimehtml.cpp \
    smtpclient/mimeinlinefile.cpp \
    smtpclient/mimemessage.cpp \
    smtpclient/mimemultipart.cpp \
    smtpclient/mimepart.cpp \
    smtpclient/mimetext.cpp \
    smtpclient/quotedprintable.cpp \
    smtpclient/smtpclient.cpp \
    smtp_set.cpp \
    answer_mod_dlg.cpp

HEADERS  += maxtest.h \
    test_results.h \
    test.h \
    third.h \
    second.h \
    admin_form.h \
    test_help.h \
    change_admin_pw_dialog.h \
    crypt.h \
    question_mod_dialog.h \
    dbfunc.h \
    theme_dlg.h \
    stud_dlg.h \
    encryption.h \
    email.h \
    email_dlg.h \
    pub_struct.h \
    smtpclient/emailaddress.h \
    smtpclient/mimeattachment.h \
    smtpclient/mimecontentformatter.h \
    smtpclient/mimefile.h \
    smtpclient/mimehtml.h \
    smtpclient/mimeinlinefile.h \
    smtpclient/mimemessage.h \
    smtpclient/mimemultipart.h \
    smtpclient/mimepart.h \
    smtpclient/mimetext.h \
    smtpclient/quotedprintable.h \
    smtpclient/smtpclient.h \
    smtpclient/smtpexports.h \
    smtpclient/SmtpMime \
    smtp_set.h \
    answer_mod_dlg.h

FORMS    += maxtest.ui \
    second.ui \
    test_results.ui \
    test.ui \
    third.ui \
    admin_form.ui \
    test_help.ui \
    change_admin_pw_dialog.ui \
    question_mod_dialog.ui \
    theme_dlg.ui \
    stud_dlg.ui \
    email_dlg.ui \
    smtp_set.ui \
    answer_mod_dlg.ui

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
    data/db_ddl.sql \
    smtpclient/readme.txt

RESOURCES += \
    res.qrc
