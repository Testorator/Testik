#ifndef CRYPT
#define CRYPT

#include <QCryptographicHash>
#include <QVariant>

QString cryptPWStr(QVariant srt_for_crypt)
{
    return QString(QCryptographicHash::hash(
                       QVariant(QCryptographicHash::hash(srt_for_crypt.toByteArray(),
                                QCryptographicHash::Sha3_512).toHex()).toByteArray(),
                                QCryptographicHash::Sha3_512).toHex());
}

#endif // CRYPT

