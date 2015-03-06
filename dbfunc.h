#ifndef DBFUNC
#define DBFUNC

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QMap>
#include <QVariant>

struct st_svMAP{
    QMap<QString,QVariant> map;
};

struct qResult{
    bool query_result;
    QString text;
    QList<st_svMAP> selection_result;
};

qResult SendSimpleQueryStr(QSqlDatabase *db, const QString& q_str);
qResult SendSimpleQueryStrWR(QSqlDatabase *db,const QString& q_str);
#endif // DBFUNC

