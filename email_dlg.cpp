#include "email_dlg.h"
#include "ui_email_dlg.h"

email_dlg::email_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::email_dlg)
{
    ui->setupUi(this);
}

email_dlg::~email_dlg()
{
    delete ui;
}

QString email_dlg::getRecipient()
{
    return ui->lineEdit_recipient->text().trimmed();
}

void email_dlg::setRecipient(QString value)
{
    ui->lineEdit_recipient->setText(value.trimmed());
}

QString email_dlg::getAddress()
{
    return ui->lineEdit_addr->text().trimmed().toLower();
}

void email_dlg::setAddress(QString value)
{
    ui->lineEdit_addr->setText(value.trimmed().toLower());
}

