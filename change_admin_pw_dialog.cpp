#include "change_admin_pw_dialog.h"
#include "ui_change_admin_pw_dialog.h"
//#include "crypt.h"
#include <QMessageBox>

change_admin_pw_dialog::change_admin_pw_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_admin_pw_dialog)
{
    ui->setupUi(this);
}
//
change_admin_pw_dialog::~change_admin_pw_dialog()
{
    delete ui;
}
//

//
void change_admin_pw_dialog::on_buttonBox_accepted()
{
    QSettings settings(QApplication::applicationDirPath().append("/maxtest.prp"),QSettings::IniFormat);

    if(ui->lineEdit_newPW->text() != ui->lineEdit_conf_newPW->text()){
        QMessageBox::critical(this,tr("Error"),tr("The new password and its confirmation do not match"));
    }
//    else if(cryptPWStr(ui->lineEdit_curPW->text()) != settings.value("crc")){
//        QMessageBox::critical(this,tr("Error"),tr("Wrong current password"));
//    }
    else{
        QMessageBox::critical(this,tr("Error"),"OK");
    }
}
//
