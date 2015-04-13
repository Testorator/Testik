#include "encryption.h"
#include "QDebug"

encryption::encryption()
{

}

encryption::~encryption()
{

}

QString encryption::stringEncrypt(QString strInput, QString strPassword)
{
    int i,j;
    QString strOutput="";

    QByteArray baInput    = strInput.toLocal8Bit();
    QByteArray baPassword = strPassword.toLocal8Bit();

    for (i=0; i < baInput.size(); i++)
    {
        for (j=0; j < baPassword.size(); j++)
        {
            baInput[i] = baInput[i] ^ (baPassword[j] + (i*j));
        }

        strOutput += QString("%1").arg((int)((unsigned char)baInput[i]),2,16,QChar('0'));
    }

    return "\'"+strOutput+"\'";
}
//
QString encryption::stringDecrypt(QVariant strInput, QString strPassword)
{
    int i,j;

    QByteArray baInput    = QByteArray::fromHex(strInput.toString().toLocal8Bit());
    QByteArray baPassword = strPassword.toLocal8Bit();

    for (i=0; i < baInput.size(); i++)
    {
        for (j=0; j < baPassword.size(); j++)
        {
            baInput[i] = baInput[i] ^ (baPassword[j] + (i*j));
        }
    }

    return QString::fromLocal8Bit(baInput.data());
}
//
