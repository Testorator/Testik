#include "answer_mod_dlg.h"
#include "ui_answer_mod_dlg.h"

answer_mod_dlg::answer_mod_dlg(bool chek_available, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::answer_mod_dlg)
{
    ui->setupUi(this);
    ui->checkBox_correct->setEnabled(chek_available);
}
//
answer_mod_dlg::~answer_mod_dlg()
{
    delete ui;
}
//
QString answer_mod_dlg::getAnswerText()
{
    return ui->textEdit_Answer->toPlainText().trimmed();
}
//
void answer_mod_dlg::setAnswerText(QString value)
{
    ui->textEdit_Answer->setText(value);
}
//
bool answer_mod_dlg::getAnswerCorrectFlag()
{
    return ui->checkBox_correct->isChecked();
}
//
void answer_mod_dlg::setAnswerCorrectFlag(bool value)
{
    ui->checkBox_correct->setChecked(value);
}
