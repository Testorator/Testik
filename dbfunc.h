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
#include "pub_struct.h"


struct st_qRes{
    bool q_result;
    QList<QMap<QString,QVariant> > sel_data;
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
    QString tabs_crypt_key,questions_crypt_key,options_crypt_key,
    answers_crypt_key,email_addreses_crypt_key;

public:
    sql_cl();
    ~sql_cl();
    encryption *crypt;
    QString students_crypt_key, groups_crypt_key,q_themes_crypt_key;
    //
    bool openDB(QString db_file);
    void closeDB();
    bool dbIsOpen();
    bool createNewDB();
    //
    bool SendSimpleQueryStr(const QString& q_str);
    st_qRes SendSimpleQueryStrWR(const QString& q_str, QString crypt_key);
    //
    QString getGroupCodeById(QString grpId);
    QVariant getGroupIdByCode(QString grpCode);
    QList<QMap<QString,QVariant> > getGroups();
    bool addGroup(const QString grpCode);
    bool clearGroup(const QVariant grpId);
    bool delGroup(const QVariant grpId);
    bool grpUnique(const QString grpCode, bool silent = 0);
    //
    QList<QMap<QString,QVariant> > getStudents(QString groupID = 0 );
    st_qRes getStudent(QString studId, QString groupID);
    bool addStudent(st_stud data);
    bool delStudent(QString studId, QString grpId = 0);
    bool studUnique(const QString Surname, const QString Name, const QString Patronymic, QString grpId = 0, bool silent = 0);
    //
    QString convertTypeOfQuestions(int type);
    QList<QMap<QString,QVariant> > getThemes();
    QList<QMap<QString,QVariant> > getThemeChild(QVariant parent_id);
    st_theme getThemeByID(QVariant theme_id);
    bool themeUnique(const QString themeName, bool silent = 0);
    bool addTheme(st_theme *new_data);
    bool updTheme(st_theme *new_data);
    bool delTheme(const QVariant theme_id);
    bool clearTheme(const QVariant theme_id);
    QList<QMap<QString,QVariant> > getQuestionsWithThemes(int questions_type);
    QList<QMap<QString,QVariant> > getQuestions(int questions_type, QString theme_id = 0);
    bool addQuest(const QString questionName, QVariant for_learn, QString theme_id, QString ans_type);
    bool questUnique(const QString questionName, bool silent = 0);
    QVariant getQuestIdByName(QString questName);
    //
    bool addAnswer(st_answer data);
    bool delAnswer(QString ans_id, QString question_id);
    QList<QMap<QString,QVariant> > getAnswers(QVariant question_id);
//   bool answerUnique(const QString ans_text, bool silent);
    //
    bool sendEMail();
    bool set_sendEMail(QVariant value);
    QList<st_email> getEMailAddreses();
    bool uniqEMailAddr(st_email *new_data);
    bool addEMailAddr(st_email *new_data);
    st_email getEMailAddressDataByRId(QString id);
    bool updEMailAddr(st_email *new_data);
    bool delEMailAddr(st_email *del_data);
    //
    bool options_hasRecords();
};
#endif // DBFUNC

