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

};

#endif // ENCRYPTION_H
