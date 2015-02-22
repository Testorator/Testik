#include "admin_form.h"
#include "ui_admin_form.h"

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
