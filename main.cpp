#include "maxtest.h"
#include "admin_form.h"
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

    admin_form af;
    Maxtest nf;

    if(argc > 1){
        for (int i = 0; i < argc; ++i){
            if(!qstrcmp(argv[i], "-adm")){
                admin_mode = true;
            }
           // qDebug() << "argv["<<i<<"]="<<argv[i];
        }
    }

    if(admin_mode){
        qDebug() << "Start in admin mode";

        QVariant in_pw = QInputDialog::getText(new QWidget,
                                           QObject::tr("Input password"),
                                           QObject::tr("Please input admin password"),
                                           QLineEdit::Password);

//        qDebug() << "pw = " << in_pw << " hash = " <<
//                    QVariant(QCryptographicHash::hash(QVariant(QCryptographicHash::hash(in_pw.toByteArray(),
//                             QCryptographicHash::Sha3_512).toHex()).toByteArray(),QCryptographicHash::Sha3_512).toHex()).toByteArray();

        QSettings settings(QApplication::applicationDirPath().append("/maxtest.prp"),QSettings::IniFormat);

//        settings.setValue("crc", cryptStr(in_pw));

        if(settings.value("crc").toString() == cryptStr(in_pw)){
            af.show();
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
        nf.show();
    }

    if(launch_app){
        return app.exec();
    }
    else {
        return 0;
    }
}

