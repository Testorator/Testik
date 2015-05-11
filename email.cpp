#include "email.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>

#include "smtpclient/SmtpMime"

bool address_correct(QString addr)
{
    QRegExp rx_std("^([a-z0-9_-]+\.)*[a-z0-9_-]+@[a-z0-9_-]+(\.[a-z0-9_-]+)*\.[a-z]{2,6}$");
    QRegExpValidator v(rx_std,0);

    int pos = 0;
    bool result = false;
    if(v.validate(addr,pos) == 2){
        result = true;
    }

    return result;
}

email::email(QObject *parent) : QObject(parent)
{

}

email::~email()
{

}
//
bool email::sendMessage(st_email *msg_data, st_smtp *smtp_data)
{
    bool result = false;

    SmtpClient smtp(smtp_data->server,smtp_data->port,SmtpClient::SslConnection);

    if(smtp_data->username.trimmed().length() > 0){
        smtp.setUser(smtp_data->username);
        smtp.setPassword(smtp_data->password);
    }

    MimeMessage message;
    message.setSender(new EmailAddress(smtp_data->mail_from));
    for(int r=0; r<msg_data->recipient.count(); r++){
        message.addRecipient(new EmailAddress(msg_data->recipient.at(r).address, msg_data->recipient.at(r).name));
    }
    message.setSubject(msg_data->msg_subj);

    MimeText text;
    text.setText(msg_data->msg_body);

    message.addPart(&text);

    result = smtp.connectToHost();
    bool connected = false;
    if(!result){
        qDebug() << smtp.getResponseText();
        QMessageBox::critical(0,tr("Error"),(smtp.getResponseText().trimmed().length() == 0) ? "Connection error" : smtp.getResponseText());
    }
    else{
        connected = true;
    }

    if(result && smtp_data->username.trimmed().length() >0){
        result = smtp.login();
        if(!result){
            qDebug() << smtp.getResponseText();
            QMessageBox::critical(0,tr("Error"),(smtp.getResponseText().trimmed().length() == 0) ? "Login error" : smtp.getResponseText());
        }
    }
    if(result){
        result = smtp.sendMail(message);
        if(!result){
            qDebug() << smtp.getResponseText();
            QMessageBox::critical(0,tr("Error"),(smtp.getResponseText().trimmed().length() == 0) ? "Send message error" : smtp.getResponseText());
        }
    }

    if(connected) smtp.quit();

    return result;
}
