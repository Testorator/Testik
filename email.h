#ifndef EMAIL_H
#define EMAIL_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>


bool address_correct(QString addr);

class email : public QObject
{
    Q_OBJECT
public:
    explicit email(QObject *parent = 0);
    ~email();
    bool sendMessage(QString *subject, QString *addreses, QString *msg);

signals:

public slots:

private:


private slots:

};

#endif // EMAIL_H
