#ifndef DBFUNC
#define DBFUNC

#include <QSqlDatabase>

bool SendSimpleQueryStr(QSqlDatabase *db, const QString& q_str);

#endif // DBFUNC

