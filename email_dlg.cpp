#include "email_dlg.h"
#include "ui_email_dlg.h"

email_dlg::email_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::email_dlg)
{
    ui->setupUi(this);
}

email_dlg::~email_dlg()
{
    delete ui;
}
