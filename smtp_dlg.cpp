#include "smtp_dlg.h"
#include "ui_smtp_dlg.h"

smtp_dlg::smtp_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::smtp_dlg)
{
    ui->setupUi(this);
}

smtp_dlg::~smtp_dlg()
{
    delete ui;
}
