#include "admin_form.h"
#include "login_dlg.h"
#include "crypt.h"
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

#include <question_mod_dialog.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    bool admin_mode = false;
    bool launch_app = true;

    admin_form *af = new admin_form();
    login_dlg *lf = new login_dlg();

    if(argc > 1){
        for (int i = 0; i < argc; ++i){
            if(!qstrcmp(argv[i], "-adm")){
                admin_mode = true;
            }
        }
    }

    if(admin_mode){
        qDebug() << "Start in admin mode";
        delete lf;
        QVariant in_pw = QInputDialog::getText(new QWidget,
                                           QObject::tr("Input password"),
                                           QObject::tr("Please input admin password"),
                                           QLineEdit::Password);

        QSettings settings(QApplication::applicationDirPath().append("/maxtest.prp"),QSettings::IniFormat);

        if(settings.value("crc").toString() == cryptStr(in_pw)){
            af->show();
        }
        else{
            qDebug() << "err: Wrong admin password.";
            QMessageBox::critical(new QWidget,"Authentication failed", "Wrong admin password");
            launch_app = false;
        }
    }
    else
    {
        qDebug() << "Start in normal mode";
        delete af;
        lf->setModal(true);
        lf->show();

    }

    if(launch_app){
        return app.exec();
    }
    else {
        return 0;
    }
}

