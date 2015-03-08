#include "add_stud_dlg.h"
#include "ui_add_stud_dlg.h"

add_stud_dlg::add_stud_dlg(stud_data *data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_stud_dlg)
{
    ui->setupUi(this);
}

add_stud_dlg::~add_stud_dlg()
{
    delete ui;
}
