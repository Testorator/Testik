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

struct st_stud {
    QString grp_code;
    QString surename;
    QString name;
    QString patronymic;
};

bool SendSimpleQueryStr(QSqlDatabase *db, const QString& q_str);
st_qRes SendSimpleQueryStrWR(QSqlDatabase *db,const QString& q_str);
//
QString getGroupCodeById(QSqlDatabase *db,QString grpId);
QVariant getGroupIdByCode(QSqlDatabase *db,QString grpCode);
QList<st_svMAP> getGroups(QSqlDatabase *db);
bool addGroup(QSqlDatabase *db, const QString grpCode);
bool clearGroup(QSqlDatabase *db, const QVariant grpId);
bool delGroup(QSqlDatabase *db, const QVariant grpId);
bool grpUnique(QSqlDatabase *db, const QString grpCode, bool silent = 0);
//
QList<st_svMAP> getStudents(QSqlDatabase *db,QString groupID = 0 );
st_qRes getStudent(QSqlDatabase *db, QString studId, QString groupID);
bool addStudent(QSqlDatabase *db,st_stud data);
bool delStudent(QSqlDatabase *db, QString studId, QString grpId = 0);
bool studUnique(QSqlDatabase *db, const QString Surename, const QString Name, const QString Patrinymic, QString grpId = 0, bool silent = 0);

#endif // DBFUNC

