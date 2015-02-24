#include "admin_form.h"
#include "ui_admin_form.h"
//#include "change_admin_pw_dialog.h"

admin_form::admin_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin_form)
{
    ui->setupUi(this);
}

admin_form::~admin_form()
{
    delete ui;
}
//
void admin_form::on_actionChange_admin_password_triggered()
{
    change_admin_pw_dialog chPW_dialog;
    chPW_dialog.exec();
}
