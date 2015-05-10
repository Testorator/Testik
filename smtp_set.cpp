#include "smtp_set.h"
#include "ui_smtp_set.h"

smtp_set::smtp_set(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::smtp_set)
{
    ui->setupUi(this);
}
//
smtp_set::~smtp_set()
{
    delete ui;
}
//
QString smtp_set::getServer()
{
    return ui->lineEdit_Server->text().trimmed();
}
//
void smtp_set::setServer(QString value)
{
    ui->lineEdit_Server->setText(value);
}
//
int smtp_set::getPort()
{
    return ui->spinBox_Port->value();
}
//
void smtp_set::setPort(int value)
{
    ui->spinBox_Port->setValue(value);
}
//
bool smtp_set::getUseSSL()
{
    return ui->checkBox_SSL->isChecked();
}
//
void smtp_set::setUseSSL(bool value)
{
    ui->checkBox_SSL->setChecked(value);
}
//
QString smtp_set::getLogin()
{
    return ui->lineEdit_Login->text().trimmed();
}
//
void smtp_set::setLogin(QString value)
{
    ui->lineEdit_Login->setText(value);
}
//
QString smtp_set::getPassword()
{
    return ui->lineEdit_Password->text().trimmed();
}
//
void smtp_set::setPassword(QString value)
{
    ui->lineEdit_Password->setText(value);
}
