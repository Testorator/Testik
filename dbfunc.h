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

class sql_cl
{
private:
    QSqlDatabase cur_db;
    QString tabs_crypt_key,questions_crypt_key,options_crypt_key,
    answers_crypt_key,email_addreses_crypt_key, tests_crypt_key, data_tests_crypt_key;

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
    QList<st_group> getGroups();
    bool addGroup(const QString grpCode);
    bool clearGroup(const QVariant grpId);
    bool delGroup(const QVariant grpId);
    bool uniqGroup(const QString grpCode, bool silent = 0);
    //
    QList<st_student> getStudents(QString groupID = 0 );
    st_qRes getStudent(QString studId, QString groupID);
    bool addStudent(st_student data);
    bool delStudent(QString studId, QString grpId = 0);
    bool uniqStudent(const QString Surname, const QString Name, const QString Patronymic, QString grpId = 0, bool silent = 0);
    //
    QString convertTypeOfQuestions(int type);
    QList<st_theme> getThemes();
    QList<st_theme> getThemeChild(QVariant parent_id);
    st_theme getThemeByID(QVariant theme_id);
    bool uniqTheme(const QString themeName, bool silent = 0);
    bool addTheme(st_theme *new_data);
    bool updTheme(st_theme *new_data);
    bool delTheme(const QVariant theme_id);
    bool clearTheme(const QVariant theme_id);
    //
    QList<QMap<QString,QVariant> > getQuestions(int questions_type, QString theme_id = 0); // FIXME: переделать чтобы возвращало QList<st_quesion>
    st_quesion getQuestionById(QVariant q_id);
    bool addQuest(const QString questionName, QVariant for_learn, QString theme_id, QString ans_type, QString comment);
    bool uniqQuestion(const QString questionName, bool silent = 0);
    QVariant getQuestIdByNameAndType(QString questName, QVariant for_learn);
    bool updateQuestion(st_quesion *new_data);
    //
    bool addAnswer(st_answer *answer);
    bool delAnswer(st_answer *data);
    QList<st_answer> getAnswers(QVariant question_id);
    st_answer getAnswerById(QVariant answer_id);
    bool uniqAnswer(const st_answer *answer, bool silent = false);
    bool updateAnswer(st_answer *new_data);
    //
    bool sendEMail();
    bool set_sendEMail(QVariant value);
    QList<st_recipient> getEMailAddreses();
    bool uniqEMailAddr(st_recipient *new_data);
    bool addEMailAddr(st_recipient *new_data);
    st_recipient getEMailAddressDataByRId(QString id);
    bool updEMailAddr(st_recipient *new_data);
    bool delEMailAddr(st_recipient *del_data);
    st_smtp getSMTP();
    bool updSMTP(st_smtp *new_data);
    //
    bool options_hasRecord(QString option_name);
};
#endif // DBFUNC

