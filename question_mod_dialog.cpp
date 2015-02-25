#include "question_mod_dialog.h"
#include "ui_question_mod_dialog.h"

question_mod_dialog::question_mod_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::question_mod_dialog)
{
    ui->setupUi(this);
}

question_mod_dialog::~question_mod_dialog()
{
    delete ui;
}
