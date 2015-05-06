#include "smtp_dlg.h"
#include "ui_smtp_dlg.h"

smtp_dlg::smtp_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::smtp_dlg)
{
    ui->setupUi(this);
    ui->lineEdit_smtp_server->setText("smtp.yandex.ru");
    ui->spinBox_smtp_port->setValue(25);
}

smtp_dlg::~smtp_dlg()
{
    delete ui;
}
//
void smtp_dlg::setPort(QVariant value)
{
    ui->spinBox_smtp_port->setValue(value.toInt());
}

//
void smtp_dlg::setServer(QVariant value)
{
    ui->lineEdit_smtp_server->setText(value.toString());
}
//
QVariant smtp_dlg::getPort()
{
    return QVariant(ui->spinBox_smtp_port->value());
}
//
QVariant smtp_dlg::getServer()
{
    return QVariant(ui->lineEdit_smtp_server->text().trimmed());
}
