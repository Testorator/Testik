#ifndef EMAIL_H
#define EMAIL_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include "pub_struct.h"


bool address_correct(QString addr);

class email : public QObject
{
    Q_OBJECT
public:
    explicit email(QObject *parent = 0);
    ~email();
    bool sendMessage(st_email *msg_data, st_smtp *smtp_data );

signals:

public slots:

private:


private slots:

};

#endif // EMAIL_H
