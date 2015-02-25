#include "admin_form_add.h"
#include "ui_admin_form_add.h"

admin_form_add::admin_form_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin_form_add)
{
    ui->setupUi(this);
}

admin_form_add::~admin_form_add()
{
    delete ui;
}
