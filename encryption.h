#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QObject>

class encryption
{
public:
    encryption();
    ~encryption();
    QString stringEncrypt(QString strInput, QString strPassword);
    QString stringDecrypt(QVariant strInput, QString strPassword);
    QList<QMap<QString,QVariant> > qresDecrypt(QList<QMap<QString,QVariant> > crypted_qres, QString crypt_key);
};

#endif // ENCRYPTION_H
