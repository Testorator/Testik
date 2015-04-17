#include "dbfunc.h"
#include "encryption.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QApplication>
#include <QTableView>


sql_cl::sql_cl()
{
    cur_db = QSqlDatabase::addDatabase("QSQLITE");
    crypt = new encryption();
    tabs_crypt_key = cur_db.databaseName();
    students_crypt_key = crypt->valueEncrypt("students",tabs_crypt_key);
    questions_crypt_key = crypt->valueEncrypt("questions",tabs_crypt_key);
    q_themes_crypt_key = crypt->valueEncrypt("q_themes",tabs_crypt_key);
    options_crypt_key = crypt->valueEncrypt("options",tabs_crypt_key);
    groups_crypt_key = crypt->valueEncrypt("groups",tabs_crypt_key);
    answers_crypt_key = crypt->valueEncrypt("answers",tabs_crypt_key);
    email_addreses_crypt_key = crypt->valueEncrypt("email_addreses",tabs_crypt_key);
}
//
sql_cl::~sql_cl()
{
    if(cur_db.isOpen()) cur_db.close();
}

//**********************************************
bool sql_cl::dbIsOpen()
{
    return cur_db.isOpen();
}
//
bool sql_cl::createNewDB()
{
    bool result = false;

    QStringList queries;
    queries.clear();
    queries.append("CREATE TABLE "+crypt->valueEncrypt("students",students_crypt_key)+
                   "("+crypt->valueEncrypt("id",students_crypt_key)+" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"+
                   crypt->valueEncrypt("group_id",students_crypt_key)+" INTEGER, "+crypt->valueEncrypt("name",students_crypt_key)+
                   " TEXT NOT NULL, "+crypt->valueEncrypt("surname",students_crypt_key)+" TEXT NOT NULL, "+
                   crypt->valueEncrypt("patronymic",students_crypt_key)+" TEXT, FOREIGN KEY ("+
                   crypt->valueEncrypt("group_id",students_crypt_key)+") REFERENCES "+crypt->valueEncrypt("groups",tabs_crypt_key)+
                   "("+crypt->valueEncrypt("id",students_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("questions",questions_crypt_key)+" ("+crypt->valueEncrypt("id",questions_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("theme_id",questions_crypt_key)+
                   "INTEGER NOT NULL, "+crypt->valueEncrypt("for_learn",questions_crypt_key)+" INTEGER NOT NULL DEFAULT 0, "+
                   crypt->valueEncrypt("question",questions_crypt_key)+" TEXT NOT NULL, FOREIGN KEY ("+
                   crypt->valueEncrypt("theme_id",questions_crypt_key)+") REFERENCES "+crypt->valueEncrypt("q_themes",questions_crypt_key)+
                   "("+crypt->valueEncrypt("id",questions_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("q_themes",q_themes_crypt_key)+" ("+crypt->valueEncrypt("id",q_themes_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("parent_id",q_themes_crypt_key)+
                   " INTEGER NOT NULL DEFAULT 0, "+crypt->valueEncrypt("name",q_themes_crypt_key)+
                   " TEXT NOT NULL UNIQUE, FOREIGN KEY ("+crypt->valueEncrypt("parent_id",q_themes_crypt_key)+") REFERENCES "+
                   crypt->valueEncrypt("q_themes",q_themes_crypt_key)+" ("+crypt->valueEncrypt("id",q_themes_crypt_key)+") );");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("options",options_crypt_key)+"("+crypt->valueEncrypt("send_report_by_email",options_crypt_key)+
                   "INTEGER NOT NULL DEFAULT 0);");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("groups",groups_crypt_key)+" ("+crypt->valueEncrypt("id",groups_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("code",groups_crypt_key)+" TEXT NOT NULL UNIQUE);");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("answers",answers_crypt_key)+" ("+crypt->valueEncrypt("id",answers_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("question_id",answers_crypt_key)+
                   " INTEGER NOT NULL, "+crypt->valueEncrypt("correct",answers_crypt_key)+" INTEGER NOT NULL DEFAULT 0, "+
                   crypt->valueEncrypt("answer",answers_crypt_key)+" TEXT, "+crypt->valueEncrypt("comment",answers_crypt_key)+
                   " TEXT, FOREIGN KEY ("+crypt->valueEncrypt("question_id",answers_crypt_key)+") REFERENCES "+crypt->valueEncrypt("question",answers_crypt_key)+
                   " ("+crypt->valueEncrypt("id",answers_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("email_addreses",email_addreses_crypt_key)+" ("+
                   crypt->valueEncrypt("recipient_name",email_addreses_crypt_key)+" TEXT NOT NULL, "+crypt->valueEncrypt("address",email_addreses_crypt_key)+
                   "TEXT NOT NULL );");
    queries.append("CREATE VIEW "+crypt->valueEncrypt("vw_test_questions",questions_crypt_key)+
                   " AS SELECT "+crypt->mdEncrypt("id",questions_crypt_key)+","+
                   crypt->mdEncrypt("theme_id",questions_crypt_key)+","+crypt->mdEncrypt("question",questions_crypt_key)
                   +" FROM "+crypt->mdEncrypt("questions",questions_crypt_key)+" WHERE "+
                   crypt->mdEncrypt("for_learn",questions_crypt_key)+" = "+crypt->valueEncrypt("0",questions_crypt_key)+";");
    queries.append("CREATE VIEW "+crypt->valueEncrypt("vw_learn_questions",questions_crypt_key)+
                   " AS SELECT "+crypt->mdEncrypt("id",questions_crypt_key)+","+crypt->mdEncrypt("theme_id",questions_crypt_key)+
                   ","+crypt->mdEncrypt("question",questions_crypt_key)+" FROM "+crypt->mdEncrypt("questions",questions_crypt_key)+
                   " WHERE "+crypt->mdEncrypt("for_learn",questions_crypt_key)+" > "+crypt->valueEncrypt("0",questions_crypt_key)+" ;");

    for(int i=0; i<queries.count();i++){
        result = SendSimpleQueryStr(queries.at(i));
        if(!result) break;
    }

    return result;
}
//
bool sql_cl::openDB(QString db_file)
{
    bool result;

    if(dbIsOpen()) cur_db.close();
    cur_db.setDatabaseName(db_file);
    cur_db.open();
    result = cur_db.isOpen();

    if(!result){
        QMessageBox msgBox;
        msgBox.setText("DB file error: "+db_file);
        msgBox.setDetailedText("Driver: "+cur_db.lastError().driverText()+
                               "\nDatabase: "+cur_db.lastError().databaseText());
        msgBox.exec();
    }
    else{
        qDebug() << "DB openinig - success";
    }
    return result;
}
//
void sql_cl::closeDB()
{
    cur_db.close();
}
//**********************************************
bool sql_cl::SendSimpleQueryStr(const QString& q_str)
{
    qDebug() << q_str;
    bool result;
    QSqlQuery *query = new QSqlQuery(cur_db);
    cur_db.transaction();
    if(!query->exec(q_str)){
        result = false;
        cur_db.rollback();
        qDebug() << query->lastError().databaseText();
        qDebug() << query->lastError().driverText();
        QMessageBox::critical(new QWidget,QObject::tr("Error"),"query: "+q_str+"\ndriver: "+
                              query->lastError().driverText()+
                              "\ndatabase: "+query->lastError().databaseText());
    }
    else{
        result = true;
        cur_db.commit();
    }
    query->clear();
    delete query;

    return result;
}
//**********************************************
QString sql_cl::convertTypeOfQuestions(int type)
{
    QString result;
    if(type == 0){
        result = "test";
    }
    else{
        result = "learn";
    }
    return result;
}

//**********************************************
st_qRes sql_cl::SendSimpleQueryStrWR(const QString& q_str, QString crypt_key)
{
//    qDebug() << q_str;
    st_qRes result;

    result.sel_data.clear();
    QList<QMap<QString,QVariant> > col;

    QSqlQuery *query = new QSqlQuery(cur_db);

    if(!query->exec(q_str)){
        cur_db.rollback();
        result.q_result = false;
        qDebug() << query->lastError().databaseText();
        qDebug() << query->lastError().driverText();
        QMessageBox::critical(new QWidget,QObject::tr("Error"),"query: "+q_str+"\ndriver: "+
                              query->lastError().driverText()+
                              "\ndatabase: "+query->lastError().databaseText());
    }
    else{
        result.q_result = true;
        QMap<QString,QVariant> col;
        QString f_name,f_value;
        while(query->next()){
            for(int counter=0; counter < query->record().count() ;counter++){
                f_name = crypt->stringDecrypt(query->record().fieldName(counter),crypt_key);
                if(f_name == "id" || f_name.contains("_id")){
                    f_value = query->value(counter).toString();
                }
                else{
                    f_value = crypt->stringDecrypt(query->value(counter),crypt_key);
                }
                col.insert(f_name,f_value);
                f_name.clear();
                f_value.clear();
            }
            result.sel_data << col;
            col.clear();
        }
    }
    query->clear();

    return result;
}
//**********************************************
// **** QUESTIONS **** {{
bool sql_cl::themeUnique(const QString themeName, bool silent)
{
    bool result;
    if(themeName.isEmpty() || themeName.isNull() || themeName.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("id",q_themes_crypt_key)+
                                             " FROM "+crypt->mdEncrypt("q_themes",q_themes_crypt_key)+" WHERE "+
                                             crypt->mdEncrypt("name",q_themes_crypt_key)+"="+
                                             crypt->valueEncrypt("themeName",q_themes_crypt_key)+";",q_themes_crypt_key);
        if(q_res.q_result){
            if(q_res.sel_data.count() > 0){
                result = false;
                if(!silent){
                    QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("Theme with name ")+
                                          "\""+themeName+"\""+QObject::tr(" already exists!"));
                }
            }
            else{
                result = true;
            }
        }
        else{
            result = false;
        }
    }
    return result;
}
//
QList<QMap<QString, QVariant> > sql_cl::getThemeByID(QVariant theme_id)
{
    QString q_themes_crypted = crypt->mdEncrypt("q_themes",q_themes_crypt_key);
    QString q_themes_id_crypted = crypt->mdEncrypt("id",q_themes_crypt_key);
    st_qRes result = SendSimpleQueryStrWR("SELECT "+q_themes_crypted+"."+q_themes_id_crypted+", "+
                                          q_themes_crypted+"."+crypt->mdEncrypt("parent_id",q_themes_crypt_key)+", "+
                                          q_themes_crypted+"."+crypt->mdEncrypt("name",q_themes_crypt_key)+
                                          " FROM "+q_themes_crypted+" WHERE "+q_themes_id_crypted+"="+theme_id.toString()+
                                          " ORDER BY "+q_themes_crypted+"."+q_themes_id_crypted,q_themes_crypt_key);
    return result.sel_data;

}
//
QList<QMap<QString, QVariant> > sql_cl::getThemes()
{
    QString q_themes_crypted = crypt->mdEncrypt("q_themes",q_themes_crypt_key);
    QString q_themes_id_crypted = crypt->mdEncrypt("id",q_themes_crypt_key);
    st_qRes result = SendSimpleQueryStrWR("SELECT "+q_themes_crypted+"."+q_themes_id_crypted+", "+
                                          q_themes_crypted+"."+crypt->mdEncrypt("parent_id",q_themes_crypt_key)+", "+
                                          q_themes_crypted+"."+crypt->mdEncrypt("name",q_themes_crypt_key)+
                                          " FROM "+q_themes_crypted+" ORDER BY "+q_themes_crypted+"."+q_themes_id_crypted,
                                          q_themes_crypt_key);
    return result.sel_data;
}

//
bool sql_cl::addTheme(const QString themeName, QString parent_id)
{
    bool result = false;
    result = themeUnique(themeName.trimmed());
    if(result){
        QString q_str = "INSERT INTO "+crypt->mdEncrypt("q_themes",q_themes_crypt_key)+" (";
        if(parent_id > 0) q_str.append(crypt->mdEncrypt("parent_id",q_themes_crypt_key)).append(",");
        q_str.append(crypt->mdEncrypt("name",q_themes_crypt_key)+") VALUES(");
        if(parent_id > 0) q_str.append(parent_id+",");
        q_str.append(crypt->valueEncrypt(themeName.trimmed(),q_themes_crypt_key)+");");
        result = SendSimpleQueryStr(q_str);
    }
    return result;
}
//
//
bool sql_cl::delTheme(const QVariant theme_id)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("q_themes",q_themes_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("id",q_themes_crypt_key)+"="+theme_id.toString());
}
//
//
bool sql_cl::clearTheme(const QVariant theme_id)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("question",questions_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("theme_id",questions_crypt_key)+"="+theme_id.toString());
}
//
QList<QMap<QString, QVariant> > sql_cl::getThemeChild(QVariant parent_id)
{
    st_qRes result = SendSimpleQueryStrWR("SELECT * FROM "+crypt->valueEncrypt("q_themes",q_themes_crypt_key)+" WHERE "+
                                          crypt->valueEncrypt("parent_id",q_themes_crypt_key)+"="+parent_id.toString()+
                                          ";",q_themes_crypt_key);
    return result.sel_data;
}
//
QList<QMap<QString, QVariant> > sql_cl::getQuestionsWithThemes(int questions_type)
{
    QString _questions_type = convertTypeOfQuestions(questions_type);

    st_qRes result = SendSimpleQueryStrWR("SELECT * FROM vw_"+_questions_type+"_questions_with_themes;","");
    return result.sel_data;
}

//
//
//bool sql_cl::addQuest(const QString questionName, QString theme_id)
//{
//    bool result = false;
//    result = questUnique(questionName.trimmed());
//    if(result){
//        QString q_str = "INSERT INTO "+crypt->mdEncrypt("questions",questions_crypt_key)+" (";
//        if(theme_id > 0) q_str.append(crypt->mdEncrypt("theme_id",questions_crypt_key)).append(",");
//        q_str.append(crypt->mdEncrypt("question",questions_crypt_key)+") VALUES(");
//        if(theme_id > 0) q_str.append(theme_id+",");
//        q_str.append(crypt->valueEncrypt(questionName.trimmed(),questions_crypt_key)+");");
//        result = SendSimpleQueryStr(q_str);
//    }
//    return result;
//}
//
QList<QMap<QString, QVariant> > sql_cl::getQuestions(int questions_type, QString theme_id)
{
    QString condition = "";

    QString cur_view = "vw_"+convertTypeOfQuestions(questions_type)+"_questions";
    QString cur_view_crypted = crypt->mdEncrypt(cur_view,questions_crypt_key);
    QString fld_id_crypted = crypt->mdEncrypt("id",questions_crypt_key);
    QString fld_theme_id_crypted = crypt->mdEncrypt("theme_id",questions_crypt_key);
    QString fld_question_crypted = crypt->mdEncrypt("question",questions_crypt_key);

    if(theme_id.trimmed().length()>0){
        condition = " WHERE "+fld_theme_id_crypted+" = "+theme_id.trimmed()+" ";
    }

    st_qRes result = SendSimpleQueryStrWR("SELECT "+cur_view_crypted+"."+fld_id_crypted+","+
                                          cur_view_crypted+"."+fld_theme_id_crypted+","+
                                          cur_view_crypted+"."+fld_question_crypted+
                                          " FROM "+cur_view_crypted+condition+"order by "+cur_view_crypted+"."+fld_id_crypted+
                                          ","+cur_view_crypted+"."+fld_question_crypted,questions_crypt_key);
    return result.sel_data;
}

// **** QUESTIONS **** }}
//**********************************************
// **** GROUPS **** {{
QList<QMap<QString,QVariant> > sql_cl::getGroups(){
    st_qRes result = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("id",groups_crypt_key)+", "+
                                          crypt->mdEncrypt("code",groups_crypt_key)+" FROM "+
                                          crypt->mdEncrypt("groups",groups_crypt_key),groups_crypt_key);

    return result.sel_data;
}
//
QString sql_cl::getGroupCodeById(QString grpId)
{
    QString result;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("code",groups_crypt_key)+" FROM "+
                                         crypt->mdEncrypt("groups",groups_crypt_key)+" WHERE "+
                                         crypt->mdEncrypt("id",groups_crypt_key)+" ="+grpId.trimmed(),groups_crypt_key);
    if(q_res.q_result){
        result = q_res.sel_data.at(0)["code"].toString();
    }
    else{
        result.clear();
    }
    return result;
}
//
QVariant sql_cl::getGroupIdByCode(QString grpCode)
{
    QVariant result;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("id",groups_crypt_key)+" FROM "+
                                         crypt->mdEncrypt("groups",groups_crypt_key)+" WHERE "+
                                         crypt->mdEncrypt("code",groups_crypt_key)+"="+
                                         crypt->valueEncrypt(grpCode.trimmed(),groups_crypt_key)+";",groups_crypt_key);
    if(q_res.q_result){
        result = q_res.sel_data.at(0)["id"];
    }
    else{
        result.clear();
    }
    return result;
}
//
bool sql_cl::addGroup(QString grpCode)
{
    bool result;
    if(sql_cl::grpUnique(grpCode)){
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("groups",groups_crypt_key)+" ("+
                                    crypt->mdEncrypt("code",groups_crypt_key)+") VALUES("+
                                    crypt->valueEncrypt(grpCode,groups_crypt_key)+");");
    }
    else{
        result = false;
    }
    return result;
}
//
bool sql_cl::clearGroup(const QVariant grpId)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("students",students_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("group_id",students_crypt_key)+"="+grpId.toString());
}
//
bool sql_cl::delGroup(const QVariant grpId)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("groups",groups_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("id",groups_crypt_key)+"="+grpId.toString());
}

//
bool sql_cl::grpUnique(const QString grpCode, bool silent)
{
    bool result;
    if(grpCode.isEmpty() || grpCode.isNull() || grpCode.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("code",groups_crypt_key)+
                                             " FROM "+crypt->mdEncrypt("groups",groups_crypt_key)+" WHERE "+
                                             crypt->mdEncrypt("code",groups_crypt_key)+"="+
                                             crypt->valueEncrypt(grpCode,groups_crypt_key)+";",groups_crypt_key);
        if(q_res.q_result){
            if(q_res.sel_data.count() > 0){
                result = false;
                if(!silent){
                    QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("Group with name ")+
                                          "\""+grpCode+"\""+QObject::tr(" already exists!"));
                }
            }
            else{
                result = true;
            }
        }
        else{
            result = false;
        }
    }
    return result;
}
// **** GROUPS **** }}
// **** STUDENTS **** {{
//DANGEROUS!! ERROR!
QList<QMap<QString, QVariant> > sql_cl::getStudents(QString groupID)
{
    st_qRes result;
    QString condition;
    if(groupID.isEmpty() || groupID.isNull() || groupID.trimmed().length() < 1){
        condition.clear();
    }
    else{
        condition = " WHERE "+crypt->mdEncrypt("group_id",students_crypt_key)+"="+groupID.trimmed();
    }
    result = SendSimpleQueryStrWR("SELECT * FROM "+crypt->mdEncrypt("students",students_crypt_key)+
                                  condition+";",students_crypt_key);

    return result.sel_data;
}
//
st_qRes sql_cl::getStudent(QString studId, QString groupID)
{
    return SendSimpleQueryStrWR("SELECT * FROM "+crypt->mdEncrypt("students",students_crypt_key)
                                +" WHERE "+crypt->mdEncrypt("id",students_crypt_key)+"="+studId+
                                " AND "+crypt->mdEncrypt("group_id",students_crypt_key)+"="+groupID+";",students_crypt_key);
}
//
bool sql_cl::addStudent(st_stud data)
{
    return SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("students",students_crypt_key)+
                              "("+crypt->mdEncrypt("group_id",students_crypt_key)+
                              ","+crypt->mdEncrypt("name",students_crypt_key)+","+
                              crypt->mdEncrypt("surname",students_crypt_key)+","+
                              crypt->mdEncrypt("patronymic",students_crypt_key)+") VALUES("+
                              getGroupIdByCode(data.grp_code).toString()+","+crypt->valueEncrypt(data.name,students_crypt_key)+","+
                              crypt->valueEncrypt(data.surname,students_crypt_key)+","+
                              crypt->valueEncrypt(data.patronymic,students_crypt_key)+");");
}

//
bool sql_cl::delStudent(QString studId, QString grpId)
{
    QString cond_grpId;
    if(grpId.isEmpty() || grpId.isNull() || grpId.trimmed().length() < 1){
        cond_grpId.clear();
    }
    else{
        cond_grpId = " AND "+crypt->mdEncrypt("group_id",students_crypt_key)+" ="+grpId;
    }
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("students",students_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("id",students_crypt_key)+"="+studId+cond_grpId+";");
}
//
bool sql_cl::studUnique(const QString Surname, const QString Name, const QString Patronymic, QString grpId, bool silent)
{
    bool result = false;
    QString cond_grpId, msg_grpId;
    if(grpId.isEmpty() || grpId.isNull() || grpId.trimmed().length() < 1){
        cond_grpId.clear();
        msg_grpId.clear();
    }
    else{
        cond_grpId = " AND "+crypt->mdEncrypt("group_id",students_crypt_key)+"="+grpId;
        msg_grpId =  QObject::tr(" in group")+" \""+getGroupCodeById(grpId)+"\"";
    }

    st_qRes q_res = SendSimpleQueryStrWR("SELECT count(*) AS stud_exists FROM "+crypt->mdEncrypt("students",students_crypt_key)+
                                         " WHERE "+crypt->mdEncrypt("name",students_crypt_key)+"="+
                                         crypt->valueEncrypt(Name,students_crypt_key)+
                                         " AND "+crypt->mdEncrypt("surname",students_crypt_key)+"="+
                                         crypt->valueEncrypt(Surname,students_crypt_key)+" AND "+
                                         crypt->mdEncrypt("patronymic",students_crypt_key)+"="+
                                         crypt->valueEncrypt(Patronymic,students_crypt_key)+cond_grpId+";",students_crypt_key);
    if(q_res.q_result){
        if(q_res.sel_data.at(0)["stud_exists"].toInt() == 0){
            result = true;
        }
        else{
            result = false;
            if(!silent){
                QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("Student")+" \""+Surname+" "+Name+" "+Patronymic+
                                      "\" "+QObject::tr("already exists")+msg_grpId+"!");
            }
        }
    }
    return result;
}
// **** STUDENTS **** }}
