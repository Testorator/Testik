#include "test_create_dlg.h"
#include "ui_test_create_dlg.h"

test_create_dlg::test_create_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::test_create_dlg)
{
    ui->setupUi(this);
}

test_create_dlg::~test_create_dlg()
{
    delete ui;
}
