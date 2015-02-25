#include "crypt.h"

#include <QCryptographicHash>

QString cryptStr(QVariant str_for_crypt)
{
    return QString(QCryptographicHash::hash(
                       QVariant(QCryptographicHash::hash(str_for_crypt.toByteArray(),
                                QCryptographicHash::Md5).toHex()).toByteArray(),
                                QCryptographicHash::Sha3_512).toHex());
}
