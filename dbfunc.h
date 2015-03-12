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

struct st_qRes{
    bool q_result;
    QList<st_svMAP> sel_data;
};

bool SendSimpleQueryStr(QSqlDatabase *db, const QString& q_str);
st_qRes SendSimpleQueryStrWR(QSqlDatabase *db,const QString& q_str);
QString getGroupCodeById(QSqlDatabase *db,QString grpId);
QList<st_svMAP> getGroups(QSqlDatabase *db);

bool addNewGroup(QSqlDatabase *db, const QString grpCode);
bool grpUnique(QSqlDatabase *db, const QString grpCode);
//
QList<st_svMAP> getStudents(QSqlDatabase *db,QString groupID = 0 );
st_qRes getStudent(QSqlDatabase *db, QString studId, QString groupID);
bool studUnique(QSqlDatabase *db, const QString Surename, const QString Name, const QString Patrinymic, QString grpId = 0);
#endif // DBFUNC

