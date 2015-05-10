#include "smtp_set.h"
#include "ui_smtp_set.h"
#include "email.h"

#include <QDateTime>

smtp_set::smtp_set(QList<st_email> *addreses, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::smtp_set)
{
    ui->setupUi(this);
    addr_list = addreses;
}
//
smtp_set::~smtp_set()
{
    delete ui;
    delete addr_list;
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
//
void smtp_set::on_toolButton_SendTestMsg_clicked()
{
    email *em = new email;

    st_email msg_data;
    for(int i=0; i<addr_list->count();i++){
        if(i>0) msg_data.recipient_address.append("; ");
        msg_data.recipient_address.append(addr_list->at(i).recipient_address);
    }

    msg_data.sender_address="akva-ymail@yandex.ru";

    msg_data.msg_body = "TEST message!";
    msg_data.msg_subj = "Test messsage from testorator ["+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")+"]";

    st_smtp smtp_data;
    smtp_data.server = getServer();
    smtp_data.port = getPort();
    smtp_data.ssl = getUseSSL();
    smtp_data.username = getLogin();
    smtp_data.password = getPassword();
    em->sendMessage(&msg_data, &smtp_data);

    delete em;
}
