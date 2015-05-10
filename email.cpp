#include "email.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>

#include "smtpclient/smtpclient.h"
#include "smtpclient/emailaddress.h"
#include "smtpclient/mimetext.h"

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
bool email::sendMessage(QString *subject, QString *addreses, QString *msg)
{
    bool result = false;

    SmtpClient smtp("smtp.yandex.ru",465,SmtpClient::SslConnection);

    smtp.setUser("akva-ymail@yandex.ru");
    smtp.setPassword("");

    MimeMessage message;
    message.setSender(new EmailAddress("akva-ymail@yandex.ru"));
    message.addRecipient(new EmailAddress(*addreses));
    message.setSubject(*subject);

    MimeText text;
    text.setText(*msg);

    message.addPart(&text);

    result = smtp.connectToHost();
    if(!result){
        qDebug() << smtp.getResponseText();
        QMessageBox::critical(0,tr("Error"),(smtp.getResponseText().trimmed().length() == 0) ? "Connection error" : smtp.getResponseText());
    }
    else{
        result = smtp.login();
        if(!result){
            qDebug() << smtp.getResponseText();
            QMessageBox::critical(0,tr("Error"),(smtp.getResponseText().trimmed().length() == 0) ? "Login error" : smtp.getResponseText());
        }
        else{
            result = smtp.sendMail(message);
            if(!result){
                qDebug() << smtp.getResponseText();
                QMessageBox::critical(0,tr("Error"),(smtp.getResponseText().trimmed().length() == 0) ? "Send message error" : smtp.getResponseText());
            }
        }
        smtp.quit();
    }
    return result;
}
