#include "email.h"
#include <QRegExpValidator>

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
bool email::sendMessage(QStringList *addreses, QString *msg)
{
    bool result = false;

    return result;
}
