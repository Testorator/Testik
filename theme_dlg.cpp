#include "theme_dlg.h"
#include "ui_theme_dlg.h"

theme_dlg::theme_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::theme_dlg)
{
    ui->setupUi(this);
}

theme_dlg::~theme_dlg()
{
    delete ui;
}
