#include "login_dlg.h"
#include "ui_login_dlg.h"

login_dlg::login_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_dlg)
{
    ui->setupUi(this);
}

login_dlg::~login_dlg()
{
    delete ui;
}
