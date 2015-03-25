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
    QString surname;
    QString name;
    QString patronymic;
};

bool openDB(QSqlDatabase *db,QString db_file);
bool isBlankDB(QSqlDatabase *db, QString db_file = 0);
bool setDB_NoBlank(QSqlDatabase *db,QString db_file);
//
bool SendSimpleQueryStr(QSqlDatabase *db, const QString& q_str);
st_qRes SendSimpleQueryStrWR(QSqlDatabase *db,const QString& q_str);
//
QString getGroupCodeById(QSqlDatabase *db,QString grpId);
QVariant getGroupIdByCode(QSqlDatabase *db,QString grpCode);
QList<st_svMAP> getGroups(QSqlDatabase *db);
bool sql_addGroup(QSqlDatabase *db, const QString grpCode);
bool sql_clearGroup(QSqlDatabase *db, const QVariant grpId);
bool sql_delGroup(QSqlDatabase *db, const QVariant grpId);
bool sql_grpUnique(QSqlDatabase *db, const QString grpCode, bool silent = 0);
//
QList<st_svMAP> getStudents(QSqlDatabase *db,QString groupID = 0 );
st_qRes sql_getStudent(QSqlDatabase *db, QString studId, QString groupID);
bool sql_addStudent(QSqlDatabase *db,st_stud data);
bool sql_delStudent(QSqlDatabase *db, QString studId, QString grpId = 0);
bool sql_studUnique(QSqlDatabase *db, const QString Surname, const QString Name, const QString Patrinymic, QString grpId = 0, bool silent = 0);
//
QList<st_svMAP> sql_getThemes(QSqlDatabase *db);
QList<st_svMAP> sql_getRootThemes(QSqlDatabase *db);
QList<st_svMAP> sql_getThemeChild(QSqlDatabase *db, QVariant parent_id);
QList<st_svMAP> sql_getThemeByID(QSqlDatabase *db, QVariant theme_id);
bool sql_themeUnique(QSqlDatabase *db, const QString themeName, bool silent = 0);
bool sql_addTheme(QSqlDatabase *db, const QString themeName, QString parent_id = 0);
QList<st_svMAP> sql_getQuestionsWithThemes(QSqlDatabase *db, int questions_type);
QList<st_svMAP> sql_getQuestions(QSqlDatabase *db,QString theme_id = 0);

#endif // DBFUNC

