#include "answer_mod_dlg.h"
#include "ui_answer_mod_dlg.h"

answer_mod_dlg::answer_mod_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::answer_mod_dlg)
{
    ui->setupUi(this);
}

answer_mod_dlg::~answer_mod_dlg()
{
    delete ui;
}
