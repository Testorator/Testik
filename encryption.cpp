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
QList<QMap<QString,QVariant> > encryption::qresDecrypt(QList<QMap<QString,QVariant> > crypted_qres,QString crypt_key)
{
    QList<QMap<QString,QVariant> > result;
    QString f_name;
    QVariant f_val;
    result.clear();
    for(int i=0;i<crypted_qres.count();i++){
        QMap<QString,QVariant> decVals;
        QMapIterator<QString, QVariant> iter(crypted_qres.at(i));
        while (iter.hasNext()){
            f_name.clear();
            f_val.clear();

            f_name = stringDecrypt(iter.key(),crypt_key);
            f_val = stringDecrypt(iter.value(),crypt_key);

            decVals.insert(f_name,f_val);
        }
        result << decVals;
    }
    return result;
}
