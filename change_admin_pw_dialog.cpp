#include "change_admin_pw_dialog.h"
#include "ui_change_admin_pw_dialog.h"

change_admin_pw_dialog::change_admin_pw_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_admin_pw_dialog)
{
    ui->setupUi(this);
}

change_admin_pw_dialog::~change_admin_pw_dialog()
{
    delete ui;
}
