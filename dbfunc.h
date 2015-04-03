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

#include <QObject>

class sql_cl
{
private:
    QSqlDatabase cur_db;
public:
    sql_cl(QSqlDatabase &db);
    ~sql_cl();
    //
    bool openDB(QString db_file);
    bool createNewDB();
    //
    bool SendSimpleQueryStr(const QString& q_str);
    st_qRes SendSimpleQueryStrWR(const QString& q_str);
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

