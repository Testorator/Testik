#include "maxtest.h"
#include "admin_form.h"
#include <QApplication>
#include <QMessageBox>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    bool admin_mode = false;

    admin_form af;
    Maxtest nf;

    qDebug() << "argc=" << argc;
    if(argc > 1){
        for (int i = 0; i < argc; ++i){
            if(!qstrcmp(argv[i], "-adm")){
                admin_mode = true;
            }
            qDebug() << "argv["<<i<<"]="<<argv[i];
        }
    }

    if(admin_mode){
        qDebug() << "Start in admin mode";
        af.show();
    }
    else
    {
        qDebug() << "Start in normal mode";
        nf.show();
    }

    return app.exec();
}
