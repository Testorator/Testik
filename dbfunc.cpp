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
    students_crypt_key = crypt->stringEncrypt("students",tabs_crypt_key);
    questions_crypt_key = crypt->stringEncrypt("questions",tabs_crypt_key);
    q_themes_crypt_key = crypt->stringEncrypt("q_themes",tabs_crypt_key);
    options_crypt_key = crypt->stringEncrypt("options",tabs_crypt_key);
    groups_crypt_key = crypt->stringEncrypt("groups",tabs_crypt_key);
    answers_crypt_key = crypt->stringEncrypt("answers",tabs_crypt_key);
    email_addreses_crypt_key = crypt->stringEncrypt("email_addreses",tabs_crypt_key);
    vw_tq_crypt_key = crypt->stringEncrypt("vw_test_questions",tabs_crypt_key);
    vw_lq_crypt_key = crypt->stringEncrypt("vw_learn_questions",tabs_crypt_key);
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
    queries.append("CREATE TABLE "+students_crypt_key+
                   "("+crypt->stringEncrypt("id",students_crypt_key)+" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"+
                   crypt->stringEncrypt("group_id",students_crypt_key)+" INTEGER, "+crypt->stringEncrypt("name",students_crypt_key)+
                   " TEXT NOT NULL, "+crypt->stringEncrypt("surname",students_crypt_key)+" TEXT NOT NULL, "+
                   crypt->stringEncrypt("patronymic",students_crypt_key)+" TEXT, FOREIGN KEY ("+
                   crypt->stringEncrypt("group_id",students_crypt_key)+") REFERENCES "+crypt->stringEncrypt("groups",tabs_crypt_key)+
                   "("+crypt->stringEncrypt("id",students_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->stringEncrypt("questions",questions_crypt_key)+" ("+crypt->stringEncrypt("id",questions_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->stringEncrypt("theme_id",questions_crypt_key)+
                   "INTEGER NOT NULL, "+crypt->stringEncrypt("for_learn",questions_crypt_key)+" INTEGER NOT NULL DEFAULT 0, "+
                   crypt->stringEncrypt("question",questions_crypt_key)+" TEXT NOT NULL, FOREIGN KEY ("+
                   crypt->stringEncrypt("theme_id",questions_crypt_key)+") REFERENCES "+crypt->stringEncrypt("q_themes",questions_crypt_key)+
                   "("+crypt->stringEncrypt("id",questions_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+" ("+crypt->stringEncrypt("id",q_themes_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->stringEncrypt("parent_id",q_themes_crypt_key)+
                   " INTEGER NOT NULL DEFAULT 0, "+crypt->stringEncrypt("name",q_themes_crypt_key)+
                   " TEXT NOT NULL UNIQUE, FOREIGN KEY ("+crypt->stringEncrypt("parent_id",q_themes_crypt_key)+") REFERENCES "+
                   crypt->stringEncrypt("q_themes",q_themes_crypt_key)+" ("+crypt->stringEncrypt("id",q_themes_crypt_key)+") );");
    queries.append("CREATE TABLE "+crypt->stringEncrypt("options",options_crypt_key)+"("+crypt->stringEncrypt("send_report_by_email",options_crypt_key)+
                   "INTEGER NOT NULL DEFAULT 0);");
    queries.append("CREATE TABLE "+crypt->stringEncrypt("groups",groups_crypt_key)+" ("+crypt->stringEncrypt("id",groups_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->stringEncrypt("code",groups_crypt_key)+" TEXT NOT NULL UNIQUE);");
    queries.append("CREATE TABLE "+crypt->stringEncrypt("answers",answers_crypt_key)+" ("+crypt->stringEncrypt("id",answers_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->stringEncrypt("question_id",answers_crypt_key)+
                   " INTEGER NOT NULL, "+crypt->stringEncrypt("correct",answers_crypt_key)+" INTEGER NOT NULL DEFAULT 0, "+
                   crypt->stringEncrypt("answer",answers_crypt_key)+" TEXT, "+crypt->stringEncrypt("comment",answers_crypt_key)+
                   " TEXT, FOREIGN KEY ("+crypt->stringEncrypt("question_id",answers_crypt_key)+") REFERENCES "+crypt->stringEncrypt("question",answers_crypt_key)+
                   " ("+crypt->stringEncrypt("id",answers_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->stringEncrypt("email_addreses",email_addreses_crypt_key)+" ("+
                   crypt->stringEncrypt("recipient_name",email_addreses_crypt_key)+" TEXT NOT NULL, "+crypt->stringEncrypt("address",email_addreses_crypt_key)+
                   "TEXT NOT NULL );");
    queries.append("CREATE VIEW "+crypt->stringEncrypt("vw_test_questions",vw_tq_crypt_key)+
                   " AS SELECT "+crypt->stringEncrypt("id",questions_crypt_key)+","+
                   crypt->stringEncrypt("theme_id",questions_crypt_key)+","+crypt->stringEncrypt("question",questions_crypt_key)
                   +" FROM "+crypt->stringEncrypt("questions",questions_crypt_key)+" WHERE "+
                   crypt->stringEncrypt("for_learn",questions_crypt_key)+" = "+crypt->stringEncrypt("0",questions_crypt_key)+";");
    queries.append("CREATE VIEW "+crypt->stringEncrypt("vw_learn_questions",vw_lq_crypt_key)+
                   " AS SELECT "+crypt->stringEncrypt("id",questions_crypt_key)+","+crypt->stringEncrypt("theme_id",questions_crypt_key)+
                   ","+crypt->stringEncrypt("question",questions_crypt_key)+" FROM "+crypt->stringEncrypt("questions",questions_crypt_key)+
                   " WHERE "+crypt->stringEncrypt("for_learn",questions_crypt_key)+" > "+crypt->stringEncrypt("0",questions_crypt_key)+" ;");

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
        while(query->next()){
            for(int counter=0; counter < query->record().count() ;counter++){
                 col.insert(query->record().fieldName(counter),query->value(counter));
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
        st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("id",q_themes_crypt_key)+
                                             " FROM "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+" WHERE "+
                                             crypt->stringEncrypt("name",q_themes_crypt_key)+"=\'"+
                                             crypt->stringEncrypt("themeName",q_themes_crypt_key)+"\';");
        // ////////DECVRYPT//////
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
    st_qRes result = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+"."+
                                          crypt->stringEncrypt("id",q_themes_crypt_key)+", "+
                                          crypt->stringEncrypt("q_themes",q_themes_crypt_key)+"."+
                                          crypt->stringEncrypt("parent_id",q_themes_crypt_key)+", "+
                                          crypt->stringEncrypt("q_themes",q_themes_crypt_key)+"."+
                                          crypt->stringEncrypt("name",q_themes_crypt_key)+
                                          "FROM "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+
                                          "WHERE ID="+theme_id.toString()+" ORDER BY "+
                                          crypt->stringEncrypt("q_themes",q_themes_crypt_key)+"."+
                                          crypt->stringEncrypt("id",q_themes_crypt_key));
    return result.sel_data;

}
//
QList<QMap<QString, QVariant> > sql_cl::getThemes()
{
    st_qRes result = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+"."+crypt->stringEncrypt("id",q_themes_crypt_key)+", "+
                                          crypt->stringEncrypt("q_themes",q_themes_crypt_key)+". "+crypt->stringEncrypt("parent_id",q_themes_crypt_key)+", "+
                                          crypt->stringEncrypt("q_themes",q_themes_crypt_key)+". "+crypt->stringEncrypt("name",q_themes_crypt_key)+
                                          "FROM "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+" ORDER BY "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+". "+
                                          crypt->stringEncrypt("id",q_themes_crypt_key));
                                          return result.sel_data;
}

//
bool sql_cl::addTheme(const QString themeName, QString parent_id) //?????????
{
    bool result = false;
    result = themeUnique(themeName.trimmed());
    if(result){
        QString q_str = "INSERT INTO "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+" (";
        if(parent_id > 0) q_str.append("parent_id,");
        q_str.append("("+crypt->stringDecrypt("name",q_themes_crypt_key)+" ) VALUES(");
        if(parent_id > 0) q_str.append(parent_id+",");
        q_str.append("\'"+crypt->stringEncrypt(themeName.trimmed(),q_themes_crypt_key)+"\');");
        result = SendSimpleQueryStr(q_str);
    }
    return result;
}
//
QList<QMap<QString, QVariant> > sql_cl::getThemeChild(QVariant parent_id)
{
    st_qRes result = SendSimpleQueryStrWR("SELECT * FROM "+crypt->stringEncrypt("q_themes",q_themes_crypt_key)+" WHERE "+
                                          crypt->stringEncrypt("parent_id",q_themes_crypt_key)+" =\'"+crypt->stringEncrypt(parent_id.toString(),q_themes_crypt_key)+"\';");
    return result.sel_data;
}
//
QList<QMap<QString, QVariant> > sql_cl::getQuestionsWithThemes(int questions_type)
{
    QString _questions_type = convertTypeOfQuestions(questions_type);

    st_qRes result = SendSimpleQueryStrWR("SELECT * \
                                          FROM vw_"+_questions_type+"_questions_with_themes;");
            return result.sel_data;
}

//
QList<QMap<QString, QVariant> > sql_cl::getQuestions(int questions_type, QString theme_id)
{
    QString _questions_type = convertTypeOfQuestions(questions_type);
    QString condition = "";
    if(theme_id.trimmed().length()>0){
        condition = "WHERE theme_id = "+theme_id.trimmed()+" ";
    }

    st_qRes result = SendSimpleQueryStrWR("SELECT vw_"+_questions_type+"_questions.id,vw_"+_questions_type+
                                          "_questions.theme_id, vw_"+_questions_type+"_questions.question \
                                          FROM vw_"+_questions_type+"_questions "+condition+
                                                                                "order by vw_"+_questions_type+"_questions.id, \
                                          vw_"+_questions_type+"_questions.question ");
                                          return result.sel_data;
}

// **** QUESTIONS **** }}
//**********************************************
// **** GROUPS **** {{
QList<QMap<QString,QVariant> > sql_cl::getGroups(){
    st_qRes result = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("id",groups_crypt_key)+", "+
                                          crypt->stringEncrypt("code",groups_crypt_key)+" FROM "+crypt->stringEncrypt("groups",groups_crypt_key));

    return result.sel_data;
}
//
QString sql_cl::getGroupCodeById(QString grpId)
{
    QString result;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("code",groups_crypt_key)+" FROM "+
                                         crypt->stringEncrypt("groups",groups_crypt_key)+" WHERE "+crypt->stringEncrypt("id",groups_crypt_key)+" ="+
                                         crypt->stringEncrypt(grpId.trimmed(),groups_crypt_key));
    if(q_res.q_result){
        result = q_res.sel_data.at(0)[crypt->stringEncrypt("code",groups_crypt_key)].toString();
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
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("id",groups_crypt_key)+" FROM "+crypt->stringEncrypt("groups",groups_crypt_key)+" WHERE "+
                                         crypt->stringEncrypt("code",groups_crypt_key)+" =\'"+crypt->stringEncrypt(grpCode.trimmed(),groups_crypt_key)+"\';");
    if(q_res.q_result){
        result = q_res.sel_data.at(0)["id"];
    }
    else{
        result.clear();
    }
    return result;
}
// НЕ РАБОТАЕТ
bool sql_cl::addGroup(QString grpCode)
{
    bool result;
    if(sql_cl::grpUnique(grpCode)){
        result = SendSimpleQueryStr("INSERT INTO "+crypt->stringEncrypt("groups",groups_crypt_key)+" ("+
                                    crypt->stringEncrypt("code",groups_crypt_key)+" ) VALUES('"+crypt->stringEncrypt(grpCode,groups_crypt_key)+"');");
    }
    else{
        result = false;
    }
    return result;
}
//
bool sql_cl::clearGroup(const QVariant grpId)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->stringEncrypt("students",students_crypt_key)+" WHERE "+crypt->stringEncrypt("group_id",students_crypt_key)+" =\'"+
                              crypt->stringEncrypt(grpId.toString(),students_crypt_key)+"\'");
}
//НЕ РАБОТАЕТ
bool sql_cl::delGroup(const QVariant grpId)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->stringEncrypt("groups",groups_crypt_key)+" WHERE "+
                              crypt->stringEncrypt("id",groups_crypt_key)+" =\'"+crypt->stringEncrypt(grpId.toString(),groups_crypt_key)+"\'");
}

//
bool sql_cl::grpUnique(const QString grpCode, bool silent)
{
    bool result;
    if(grpCode.isEmpty() || grpCode.isNull() || grpCode.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->stringEncrypt("code",groups_crypt_key)+" FROM "+crypt->stringEncrypt("groups",groups_crypt_key)+" WHERE "+
                                             crypt->stringEncrypt("code",groups_crypt_key)+"='"+grpCode+"';");
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
QList<QMap<QString, QVariant> > sql_cl::getStudents(QString groupID) //как делать??
{
    st_qRes result;
    QString condition;
    if(groupID.isEmpty() || groupID.isNull() || groupID.trimmed().length() < 1){
        condition.clear();
    }
    else{
        condition = " WHERE "+crypt->stringEncrypt("group_id",students_crypt_key)+" ='"+crypt->stringEncrypt(groupID.trimmed(),students_crypt_key)+"' ";
    }
    result = SendSimpleQueryStrWR("SELECT * FROM students"+condition+";");

    return result.sel_data;
}
//
st_qRes sql_cl::getStudent(QString studId, QString groupID) //как делать??
{
    return SendSimpleQueryStrWR("SELECT * FROM students WHERE id="+studId+
                                " AND group_id="+groupID+";");
}
//
bool sql_cl::addStudent(st_stud data)
{
    return SendSimpleQueryStr("INSERT INTO students(group_id,name,surname,patronymic) VALUES("+
                              getGroupIdByCode(data.grp_code).toString()+",\'"+data.name+"\',\'"+
                              data.surname+"\',\'"+data.patronymic+"\');");
}

//
bool sql_cl::delStudent(QString studId, QString grpId)
{
    QString cond_grpId;
    if(grpId.isEmpty() || grpId.isNull() || grpId.trimmed().length() < 1){
        cond_grpId.clear();
    }
    else{
        cond_grpId = " AND group_id="+grpId;
    }
    return SendSimpleQueryStr("DELETE FROM students WHERE id="+studId+cond_grpId+";");
}
//
bool sql_cl::studUnique(const QString Surname, const QString Name, const QString Patrinymic, QString grpId, bool silent)
{
    bool result = false;
    QString cond_grpId, msg_grpId;
    if(grpId.isEmpty() || grpId.isNull() || grpId.trimmed().length() < 1){
        cond_grpId.clear();
        msg_grpId.clear();
    }
    else{
        cond_grpId = " AND group_id="+grpId;
        msg_grpId =  QObject::tr(" in group")+" \""+getGroupCodeById(grpId)+"\"";
    }

    st_qRes q_res = SendSimpleQueryStrWR("SELECT count(*) AS stud_exists FROM students WHERE name=\'"+Name+
                                         "\' AND surname=\'"+Surname+"\' AND patronymic=\'"+Patrinymic+"\'"+
                                         cond_grpId+";");
    if(q_res.q_result){
        if(q_res.sel_data.at(0)["stud_exists"].toInt() == 0){
            result = true;
        }
        else{
            result = false;
            if(!silent){
                QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("Student")+" \""+Surname+" "+Name+" "+Patrinymic+
                                      "\" "+QObject::tr("already exists")+msg_grpId+"!");
            }
        }
    }
    return result;
}
// **** STUDENTS **** }}
