#ifndef DBFUNC
#define DBFUNC

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QMap>
#include <QVariant>
#include "encryption.h"

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



class sql_cl
{
private:
    QSqlDatabase cur_db;
    QString tabs_crypt_key,students_crypt_key,questions_crypt_key,q_themes_crypt_key,options_crypt_key,groups_crypt_key,
    answers_crypt_key,email_addreses_crypt_key,vw_tq_crypt_key,vw_lq_crypt_key;
public:
    sql_cl();
    ~sql_cl();
    encryption *crypt;
    //
    bool openDB(QString db_file);
    void closeDB();
    bool dbIsOpen();
    bool createNewDB();
    //
    bool SendSimpleQueryStr(const QString& q_str);
    st_qRes SendSimpleQueryStrWR(const QString& q_str, QString crypt_key = 0 );
    //
    QString getGroupCodeById(QString grpId);
    QVariant getGroupIdByCode(QString grpCode);
    QList<st_svMAP> getGroups();
    bool addGroup(const QString grpCode);
    bool clearGroup(const QVariant grpId);
    bool delGroup(const QVariant grpId);
    bool grpUnique(const QString grpCode, bool silent = 0);
    //
    QList<st_svMAP> getStudents(QString groupID = 0 );
    st_qRes getStudent(QString studId, QString groupID);
    bool addStudent(st_stud data);
    bool delStudent(QString studId, QString grpId = 0);
    bool studUnique(const QString Surname, const QString Name, const QString Patrinymic, QString grpId = 0, bool silent = 0);
    //
    QString convertTypeOfQuestions(int type);
    QList<st_svMAP> getThemes();
    QList<st_svMAP> getThemeChild(QVariant parent_id);
    QList<st_svMAP> getThemeByID(QVariant theme_id);
    bool themeUnique(const QString themeName, bool silent = 0);
    bool addTheme(const QString themeName, QString parent_id = 0);
    QList<st_svMAP> getQuestionsWithThemes(int questions_type);
    QList<st_svMAP> getQuestions(int questions_type, QString theme_id = 0);
};
#endif // DBFUNC

