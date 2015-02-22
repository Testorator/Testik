#include "maxtest.h"
#include "admin_form.h"
#include <QApplication>
#include <QMessageBox>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if(argc == 1){
        Maxtest w;
        w.show();
    }
    else
    {
        for (int i = 1; i < argc; ++i){
            if(!qstrcmp(argv[i], "-adm")){
                qDebug() << "Start in admin mode";
           //     admin_form af;
           //     af.show();
            }
        }
    }
    return app.exec();
}
