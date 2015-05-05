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
    bool sendMessage(QString *subject, QStringList *addreses, QString *msg);

signals:

public slots:

private:
    QString message;
    QTextStream *t;
    QTcpSocket *socket;
    QString from;
    QString rcpt;
    QString response;
    enum states{Rcpt,Mail,Data,Init,Body,Quit,Close};
    int state;

private slots:
    void stateChanged(QTcpSocket::SocketState socketState);
    void errorReceived(QTcpSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();
};

#endif // EMAIL_H
