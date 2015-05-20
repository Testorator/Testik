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
                   crypt->valueEncrypt("question",questions_crypt_key)+" TEXT NOT NULL, "+crypt->valueEncrypt("answer_type",questions_crypt_key)+" TEXT, "+
                   crypt->valueEncrypt("comment",questions_crypt_key)+" TEXT, FOREIGN KEY ("+
                   crypt->valueEncrypt("theme_id",questions_crypt_key)+") REFERENCES "+crypt->valueEncrypt("q_themes",questions_crypt_key)+
                   "("+crypt->valueEncrypt("id",questions_crypt_key)+"));");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("q_themes",q_themes_crypt_key)+" ("+crypt->valueEncrypt("id",q_themes_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("parent_id",q_themes_crypt_key)+
                   " INTEGER NOT NULL DEFAULT 0, "+crypt->valueEncrypt("name",q_themes_crypt_key)+
                   " TEXT NOT NULL UNIQUE, FOREIGN KEY ("+crypt->valueEncrypt("parent_id",q_themes_crypt_key)+") REFERENCES "+
                   crypt->valueEncrypt("q_themes",q_themes_crypt_key)+" ("+crypt->valueEncrypt("id",q_themes_crypt_key)+") );");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("options",options_crypt_key)+"("+crypt->valueEncrypt("opt_name",options_crypt_key)+
                   " TEXT, "+crypt->valueEncrypt("value",options_crypt_key)+" TEXT );");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("groups",groups_crypt_key)+" ("+crypt->valueEncrypt("id",groups_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("code",groups_crypt_key)+" TEXT NOT NULL UNIQUE);");
    queries.append("CREATE TABLE "+crypt->valueEncrypt("answers",answers_crypt_key)+" ("+crypt->valueEncrypt("id",answers_crypt_key)+
                   " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "+crypt->valueEncrypt("question_id",answers_crypt_key)+
                   " INTEGER NOT NULL, "+crypt->valueEncrypt("correct",answers_crypt_key)+" INTEGER NOT NULL DEFAULT 0, "+crypt->valueEncrypt("answer",answers_crypt_key)+
                   " TEXT, FOREIGN KEY ("+crypt->valueEncrypt("question_id",answers_crypt_key)+
                   ") REFERENCES "+crypt->valueEncrypt("question",answers_crypt_key)+" ("+crypt->valueEncrypt("id",answers_crypt_key)+"));");
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
    //        qDebug() << q_str;
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
    //        qDebug() << q_str;
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
                if(query->record().fieldName(counter) != "rowid"){
                    f_name = crypt->stringDecrypt(query->record().fieldName(counter),crypt_key);
                }
                else{
                    f_name = query->record().fieldName(counter);
                }
                if(f_name == "rowid" || f_name == "id" || f_name.contains("_id")){
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
bool sql_cl::uniqTheme(const QString themeName, bool silent)
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
st_theme sql_cl::getThemeByID(QVariant theme_id)
{
    st_theme result;
    QString q_themes_crypted = crypt->mdEncrypt("q_themes",q_themes_crypt_key);
    QString q_themes_id_crypted = crypt->mdEncrypt("id",q_themes_crypt_key);

    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+q_themes_crypted+"."+q_themes_id_crypted+", "+
                                         q_themes_crypted+"."+crypt->mdEncrypt("parent_id",q_themes_crypt_key)+", "+
                                         q_themes_crypted+"."+crypt->mdEncrypt("name",q_themes_crypt_key)+
                                         " FROM "+q_themes_crypted+" WHERE "+q_themes_id_crypted+"="+theme_id.toString()+";",q_themes_crypt_key);

    if(q_res.q_result){
        result.id = q_res.sel_data.at(0)["id"].toString();
        result.name = q_res.sel_data.at(0)["name"].toString();
        result.parent_id = q_res.sel_data.at(0)["parent_id"].toString();
    }
    return result;
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
bool sql_cl::addTheme(st_theme *new_data)
{
    bool result = false;
    result = uniqTheme(new_data->name);
    if(result){
        QString q_str = "INSERT INTO "+crypt->mdEncrypt("q_themes",q_themes_crypt_key)+" (";
        if(new_data->parent_id > 0) q_str.append(crypt->mdEncrypt("parent_id",q_themes_crypt_key)).append(",");
        q_str.append(crypt->mdEncrypt("name",q_themes_crypt_key)+") VALUES(");
        if(new_data->parent_id > 0) q_str.append(new_data->parent_id+",");
        q_str.append(crypt->valueEncrypt(new_data->name,q_themes_crypt_key)+");");
        result = SendSimpleQueryStr(q_str);
    }
    return result;
}
//
bool sql_cl::updTheme(st_theme *new_data)
{
    bool result = false;
    st_theme db_data;
    db_data = getThemeByID(new_data->id);
    QString q_str = "UPDATE "+crypt->mdEncrypt("q_themes",q_themes_crypt_key)+" SET ";
    QString upd_data;

    if(new_data->name != db_data.name){
        upd_data.append(crypt->mdEncrypt("name",q_themes_crypt_key)+"="+crypt->valueEncrypt(new_data->name,q_themes_crypt_key));
    }
    if(new_data->parent_id != db_data.parent_id){
        if(upd_data.length() > 0) upd_data.append(", ");
        upd_data.append(crypt->mdEncrypt("parent_id",q_themes_crypt_key)+"="+new_data->parent_id);
    }

    if(upd_data.trimmed().length() > 0){
        q_str +=upd_data+" WHERE "+crypt->mdEncrypt("id",q_themes_crypt_key)+"="+db_data.id+
                " AND "+crypt->mdEncrypt("name",q_themes_crypt_key)+"="+crypt->valueEncrypt(db_data.name,q_themes_crypt_key)+
                " AND "+crypt->mdEncrypt("parent_id",q_themes_crypt_key)+"="+db_data.parent_id+";";
        result = SendSimpleQueryStr(q_str);
    }

    return result;
}
//
bool sql_cl::delTheme(const QVariant theme_id)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("q_themes",q_themes_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("id",q_themes_crypt_key)+"="+theme_id.toString());
}
//
bool sql_cl::clearTheme(const QVariant theme_id)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("questions",questions_crypt_key)+
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
bool sql_cl::addQuest(const QString questionName, QVariant for_learn, QString theme_id, QString ans_type, QString comment)
{
    bool result = false;
    result = uniqQuestion(questionName.trimmed());
    if(result){
        QString q_str = "INSERT INTO "+crypt->mdEncrypt("questions",questions_crypt_key)+" ("+crypt->mdEncrypt("theme_id",questions_crypt_key)+","+
                crypt->valueEncrypt("for_learn",questions_crypt_key)+","+crypt->mdEncrypt("question",questions_crypt_key)+","+
                crypt->valueEncrypt("answer_type",questions_crypt_key)+","+crypt->valueEncrypt("comment",questions_crypt_key)+") VALUES("+theme_id+","+
                crypt->valueEncrypt(for_learn.toString(),questions_crypt_key)+","+crypt->valueEncrypt(questionName.trimmed(),questions_crypt_key)+","+
                crypt->valueEncrypt(ans_type,questions_crypt_key)+","+ crypt->valueEncrypt(comment.trimmed(),questions_crypt_key)+");";
        result = SendSimpleQueryStr(q_str);
    }
    return result;
}
//
bool sql_cl::uniqQuestion(const QString questionName, bool silent)
{
    bool result;
    if(questionName.isEmpty() || questionName.isNull() || questionName.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("question",questions_crypt_key)+
                                             " FROM "+crypt->mdEncrypt("questions",questions_crypt_key)+" WHERE "+
                                             crypt->mdEncrypt("question",questions_crypt_key)+"="+
                                             crypt->valueEncrypt(questionName,questions_crypt_key)+";",questions_crypt_key);
        if(q_res.q_result){
            if(q_res.sel_data.count() > 0){
                result = false;
                if(!silent){
                    QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("This question ")+
                                          "\""+questionName+"\""+QObject::tr(" already exists!"));
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

    st_qRes result = SendSimpleQueryStrWR("SELECT "+fld_id_crypted+","+fld_theme_id_crypted+","+fld_question_crypted+
                                          " FROM "+cur_view_crypted+condition+"order by "+cur_view_crypted+"."+fld_id_crypted+
                                          ","+cur_view_crypted+"."+fld_question_crypted,questions_crypt_key);
    return result.sel_data;
}
//
QVariant sql_cl::getQuestIdByNameAndType(QString questName, QVariant for_learn)
{
    QVariant result;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("id",questions_crypt_key)+" FROM "+
                                         crypt->mdEncrypt("questions",questions_crypt_key)+" WHERE "+
                                         crypt->mdEncrypt("question",questions_crypt_key)+"="+
                                         crypt->valueEncrypt(questName.trimmed(),questions_crypt_key)+
                                         " AND "+crypt->mdEncrypt("for_learn",questions_crypt_key)+"="+
                                         crypt->valueEncrypt(for_learn.toString(),questions_crypt_key)+";",questions_crypt_key);
    if(q_res.q_result){
        result = q_res.sel_data.at(0)["id"];
    }
    else{
        result.clear();
    }
    return result;
}
//
st_quesion sql_cl::getQuestionById(QVariant q_id)
{
    st_quesion result;

    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("id",questions_crypt_key)+","+crypt->mdEncrypt("theme_id",questions_crypt_key)+
                                         ","+crypt->mdEncrypt("question",questions_crypt_key)+","+crypt->mdEncrypt("answer_type",questions_crypt_key)+
                                         ","+crypt->mdEncrypt("comment",questions_crypt_key)+" FROM "+crypt->mdEncrypt("questions",questions_crypt_key)+" WHERE "+
                                         crypt->mdEncrypt("id",questions_crypt_key)+"="+q_id.toString()+";",questions_crypt_key);
    if(q_res.q_result){
        result.id = q_res.sel_data.at(0)["id"].toString();
        result.theme_id = q_res.sel_data.at(0)["theme_id"].toString();
        result.text = q_res.sel_data.at(0)["question"].toString();
        result.comment = q_res.sel_data.at(0)["comment"].toString();
        result.ans_type = q_res.sel_data.at(0)["answer_type"].toInt();
    }

    return result;
}
//
bool sql_cl::updateQuestion(st_quesion *new_data)
{
    bool result = false;
    st_quesion db_data = getQuestionById(new_data->id);
    bool upd = false;
    QString query_str = "UPDATE "+crypt->valueEncrypt("questions",questions_crypt_key)+" SET ";
    if(new_data->text != db_data.text){
        upd = true;
        query_str.append(crypt->valueEncrypt("question",questions_crypt_key)+"="+crypt->valueEncrypt(new_data->text,questions_crypt_key));
    };
    if(new_data->comment != db_data.comment){
        upd = true;
        query_str.append(crypt->valueEncrypt("comment",questions_crypt_key)+"="+crypt->valueEncrypt(new_data->comment,questions_crypt_key));
    };
    if(new_data->ans_type != db_data.ans_type){
        upd = true;
        query_str.append(crypt->valueEncrypt("answer_type",questions_crypt_key)+"="+crypt->valueEncrypt(QVariant(new_data->ans_type).toString(),questions_crypt_key));
    }

    if(upd){
        if(uniqQuestion(new_data->text)){
            query_str.append(" WHERE "+crypt->mdEncrypt("id",questions_crypt_key)+"="+new_data->id+";");
            result = SendSimpleQueryStr(query_str);
        }
    }
    return result;
}
//
// **** QUESTIONS **** }}
//**********************************************
// **** ANSWERS **** {{
//
bool sql_cl::addAnswer(st_answer *answer)
{
    bool result = false;

    if(uniqAnswer(answer)){
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("answers",answers_crypt_key)+"("+crypt->mdEncrypt("question_id",answers_crypt_key)+
                                    ","+crypt->mdEncrypt("answer",answers_crypt_key)+","+crypt->mdEncrypt("correct",answers_crypt_key)+" ) VALUES("+
                                    answer->question_id+","+crypt->valueEncrypt(answer->ans_text,answers_crypt_key)+","+
                                    crypt->valueEncrypt(QVariant(answer->ans_correct).toString(),answers_crypt_key)+");");
    }
    return result;
}
//
bool sql_cl::delAnswer(QString ans_id, QString question_id)
{

    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("answers",answers_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("id",answers_crypt_key)+"="+ans_id+
                              " AND "+crypt->mdEncrypt("question_id",answers_crypt_key)+"="+question_id+";");
}
//
QList<st_answer> sql_cl::getAnswers(QVariant question_id)
{
    QList<st_answer> result;
    st_qRes q_result = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("id",answers_crypt_key)+", "+crypt->mdEncrypt("question_id",answers_crypt_key)
                                            +", "+crypt->mdEncrypt("correct",answers_crypt_key)+", "+crypt->mdEncrypt("answer",answers_crypt_key)
                                            +" FROM "+crypt->mdEncrypt("answers",answers_crypt_key)+
                                            " WHERE "+crypt->mdEncrypt("question_id",answers_crypt_key)+"="+question_id.toString()+";",answers_crypt_key);

    if(q_result.q_result){
        result.clear();
        for(int i=0; i<q_result.sel_data.count(); i++){
            st_answer ans_from_db;
            ans_from_db.question_id = question_id.toString();
            ans_from_db.ans_id = q_result.sel_data.at(i)["id"].toString();
            ans_from_db.ans_text = q_result.sel_data.at(i)["answer"].toString();
            ans_from_db.ans_correct = q_result.sel_data.at(i)["correct"].toBool();
            result.append(ans_from_db);
        }
    }

    return result;
}
//
bool sql_cl::uniqAnswer(const st_answer *answer, bool silent)
{
    bool result;
    if(answer->ans_text.isEmpty() || answer->ans_text.isNull() || answer->ans_text.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("answer",answers_crypt_key)+
                                             " FROM "+crypt->mdEncrypt("answers",answers_crypt_key)+" WHERE "+
                                             crypt->mdEncrypt("answer",answers_crypt_key)+"="+crypt->valueEncrypt(answer->ans_text,answers_crypt_key)+
                                             " AND "+crypt->mdEncrypt("question_id",answers_crypt_key)+"="+answer->question_id+
                                             ";",answers_crypt_key);
        if(q_res.q_result){
            if(q_res.sel_data.count() > 0){
                result = false;
                if(!silent){
                    QString ans_text;
                    if(answer->ans_text.trimmed().length()>30){
                        ans_text=answer->ans_text.left(20)+"...."+answer->ans_text.right(10);
                    }
                    else{
                        ans_text = answer->ans_text;
                    }
                    QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("This answer ")+
                                          "\""+ans_text+"\""+QObject::tr(" already exists!"));
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
// **** ANSWERS **** }}
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
    if(sql_cl::uniqGroup(grpCode)){
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
bool sql_cl::uniqGroup(const QString grpCode, bool silent)
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
bool sql_cl::uniqStudent(const QString Surname, const QString Name, const QString Patronymic, QString grpId, bool silent)
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
// **** OPTIONS **** {{
bool sql_cl::options_hasRecord(QString option_name)
{
    bool result = false;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("opt_name",options_crypt_key)+
                                         " FROM "+crypt->mdEncrypt("options",options_crypt_key)+
                                         " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+
                                         crypt->valueEncrypt(option_name,options_crypt_key),options_crypt_key);

    if(q_res.q_result){
        if(q_res.sel_data.count() > 0){
            result = true;
        }
        else{
            result = false;
        }
    }
    return result;
}

// **** OPTIONS **** }}
// **** EMAIL **** {{
bool sql_cl::sendEMail()
{
    bool result = false;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("opt_name",options_crypt_key)+", "+
                                         crypt->mdEncrypt("value",options_crypt_key)+" FROM "+
                                         crypt->mdEncrypt("options",options_crypt_key)+
                                         " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+
                                         crypt->valueEncrypt("smtp_send_mail",options_crypt_key),options_crypt_key);

    if(q_res.q_result && q_res.sel_data.count() > 0 ){
        if(q_res.sel_data.at(0)["value"].toInt() == 1){
            result = true;
        }
        else{
            result = false;
        }
    }
    return result;
}
//
bool sql_cl::set_sendEMail(QVariant value)
{
    bool result;
    if(options_hasRecord("smtp_send_mail")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+
                                    crypt->valueEncrypt(QVariant(value.toInt()).toString(),options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+
                                    crypt->valueEncrypt("smtp_send_mail",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+
                                    crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+
                                    crypt->valueEncrypt("smtp_send_mail",options_crypt_key)+","+
                                    crypt->valueEncrypt(QVariant(value.toInt()).toString(),options_crypt_key)+");");
    }
    return result;
}
//
QList<st_recipient> sql_cl::getEMailAddreses()
{
    QList<st_recipient> result;

    st_qRes sql_res = SendSimpleQueryStrWR("SELECT rowid,"+crypt->mdEncrypt("recipient_name",email_addreses_crypt_key)+","+
                                           crypt->mdEncrypt("address",email_addreses_crypt_key)+
                                           " FROM "+crypt->mdEncrypt("email_addreses",email_addreses_crypt_key)+";",email_addreses_crypt_key);

    if(sql_res.q_result){
        for(int i=0; i<sql_res.sel_data.count(); i++){
            st_recipient addr;
            addr.id = sql_res.sel_data.at(i)["rowid"].toString();
            addr.address = sql_res.sel_data.at(i)["address"].toString();
            addr.name = sql_res.sel_data.at(i)["recipient_name"].toString();
            result.append(addr);
        }
    }

    return result;
}

//
bool sql_cl::uniqEMailAddr(st_recipient *new_data)
{
    bool result = false;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT * FROM "+crypt->mdEncrypt("email_addreses",email_addreses_crypt_key)+
                                         " WHERE "+crypt->mdEncrypt("recipient_name",email_addreses_crypt_key)+"="+crypt->valueEncrypt(new_data->name,email_addreses_crypt_key)+
                                         " OR "+crypt->mdEncrypt("address",email_addreses_crypt_key)+"="+crypt->valueEncrypt(new_data->address,email_addreses_crypt_key),email_addreses_crypt_key);

    if(q_res.q_result){
        if(q_res.sel_data.count() == 0){
            result = true;
        }
        else{
            result = false;
            QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("Address")+" "+new_data->address+" "+QObject::tr("or")+" "+
                                  QObject::tr("recipient with name")+"\" "+new_data->name+"\" "+QObject::tr("already exists")+"!");
        }
    }

    return result;
}
//
bool sql_cl::addEMailAddr(st_recipient *new_data)
{
    bool result = false;
    if(uniqEMailAddr(new_data)){
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("email_addreses",email_addreses_crypt_key)+"("+crypt->mdEncrypt("recipient_name",email_addreses_crypt_key)+","+
                                    crypt->mdEncrypt("address",email_addreses_crypt_key)+") VALUES("+crypt->valueEncrypt(new_data->name,email_addreses_crypt_key)+","+
                                    crypt->valueEncrypt(new_data->address,email_addreses_crypt_key)+")");
    }

    return result;
}
//
st_recipient sql_cl::getEMailAddressDataByRId(QString id)
{
    st_recipient result;

    st_qRes sql_result = SendSimpleQueryStrWR("SELECT * FROM "+crypt->mdEncrypt("email_addreses",email_addreses_crypt_key)+
                                              "WHERE rowid="+id+";",email_addreses_crypt_key);
    if(sql_result.sel_data.count() > 0){
        result.name = sql_result.sel_data.at(0)["recipient_name"].toString();
        result.address = sql_result.sel_data.at(0)["address"].toString();
    }

    return result;
}
//
bool sql_cl::updEMailAddr(st_recipient *new_data)
{
    bool result = false;
    st_recipient upd_data;
    st_recipient db_data = getEMailAddressDataByRId(new_data->id);
    bool upd = false;
    QString query_str = "UPDATE "+crypt->mdEncrypt("email_addreses",email_addreses_crypt_key)+" SET ";
    if(new_data->address != db_data.address){
        upd = true;
        upd_data.address = new_data->address;
        query_str.append(crypt->mdEncrypt("address",email_addreses_crypt_key)+"="+crypt->valueEncrypt(new_data->address,email_addreses_crypt_key));
    };
    if(new_data->name != db_data.name){
        upd = true;
        upd_data.name = new_data->name;
        query_str.append(crypt->mdEncrypt("recipient_name",email_addreses_crypt_key)+"="+crypt->valueEncrypt(new_data->name,email_addreses_crypt_key));
    };
    query_str.append(" WHERE rowid="+new_data->id+";");

    if(uniqEMailAddr(&upd_data)){
        if(upd){
            result = SendSimpleQueryStr(query_str);
        }
    }
    return result;
}
//
bool sql_cl::delEMailAddr(st_recipient *del_data)
{
    return SendSimpleQueryStr("DELETE FROM "+crypt->mdEncrypt("email_addreses",email_addreses_crypt_key)+
                              " WHERE "+crypt->mdEncrypt("address",email_addreses_crypt_key)+"="+crypt->valueEncrypt(del_data->address,email_addreses_crypt_key)+
                              " AND "+crypt->mdEncrypt("recipient_name",email_addreses_crypt_key)+"="+crypt->valueEncrypt(del_data->name,email_addreses_crypt_key)+
                              " AND rowid="+del_data->id);
}
//
st_smtp sql_cl::getSMTP()
{
    st_smtp result;

    st_qRes sql_result = SendSimpleQueryStrWR("SELECT "+crypt->mdEncrypt("opt_name",options_crypt_key)+", "+crypt->mdEncrypt("value",options_crypt_key)+
                                              " FROM "+crypt->mdEncrypt("options",options_crypt_key)+
                                              " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_server",options_crypt_key)+
                                              " OR "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_port",options_crypt_key)+
                                              " OR "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_mail_from",options_crypt_key)+
                                              " OR "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_username",options_crypt_key)+
                                              " OR "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_password",options_crypt_key)+
                                              " OR "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_ssl",options_crypt_key)+
                                              ";",options_crypt_key);
    for(int i=0; i<sql_result.sel_data.count(); i++){
        if(sql_result.sel_data.at(i)["opt_name"].toString() == "smtp_server"){
            result.server = sql_result.sel_data.at(i)["value"].toString();
        }
        else if(sql_result.sel_data.at(i)["opt_name"].toString() == "smtp_port"){
            result.port = sql_result.sel_data.at(i)["value"].toInt();
        }
        else if(sql_result.sel_data.at(i)["opt_name"].toString() == "smtp_mail_from"){
            result.mail_from = sql_result.sel_data.at(i)["value"].toString();
        }
        else if(sql_result.sel_data.at(i)["opt_name"].toString() == "smtp_username"){
            result.username = sql_result.sel_data.at(i)["value"].toString();
        }
        else if(sql_result.sel_data.at(i)["opt_name"].toString() == "smtp_password"){
            result.password = sql_result.sel_data.at(i)["value"].toString();
        }
        else if(sql_result.sel_data.at(i)["opt_name"].toString() == "smtp_ssl"){
            result.ssl = sql_result.sel_data.at(i)["value"].toBool();
        }
    }
    return result;
}
//
bool sql_cl::updSMTP(st_smtp *new_data)
{
    bool result = false;

    if(options_hasRecord("smtp_server")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+crypt->valueEncrypt(new_data->server,options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_server",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+crypt->valueEncrypt("smtp_server",options_crypt_key)+","+
                                    crypt->valueEncrypt(new_data->server,options_crypt_key)+");");
    }

    if(options_hasRecord("smtp_port")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+crypt->valueEncrypt(QVariant(new_data->port).toString(),options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_port",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+crypt->valueEncrypt("smtp_port",options_crypt_key)+","+
                                    crypt->valueEncrypt(QVariant(new_data->port).toString(),options_crypt_key)+");");
    }

    if(options_hasRecord("smtp_mail_from")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+crypt->valueEncrypt(new_data->mail_from,options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_mail_from",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+crypt->valueEncrypt("smtp_mail_from",options_crypt_key)+","+
                                    crypt->valueEncrypt(new_data->mail_from,options_crypt_key)+");");
    }

    if(options_hasRecord("smtp_username")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+crypt->valueEncrypt(new_data->username,options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_username",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+crypt->valueEncrypt("smtp_username",options_crypt_key)+","+
                                    crypt->valueEncrypt(new_data->username,options_crypt_key)+");");
    }

    if(options_hasRecord("smtp_password")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+crypt->valueEncrypt(new_data->password,options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_password",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+crypt->valueEncrypt("smtp_password",options_crypt_key)+","+
                                    crypt->valueEncrypt(new_data->password,options_crypt_key)+");");
    }

    if(options_hasRecord("smtp_ssl")){
        result = SendSimpleQueryStr("UPDATE "+crypt->mdEncrypt("options",options_crypt_key)+" SET "+
                                    crypt->mdEncrypt("value",options_crypt_key)+"="+crypt->valueEncrypt(QVariant(new_data->ssl).toString(),options_crypt_key)+
                                    " WHERE "+crypt->mdEncrypt("opt_name",options_crypt_key)+"="+crypt->valueEncrypt("smtp_ssl",options_crypt_key)+";");
    }
    else{
        result = SendSimpleQueryStr("INSERT INTO "+crypt->mdEncrypt("options",options_crypt_key)+"("+crypt->mdEncrypt("opt_name",options_crypt_key)+","+
                                    crypt->mdEncrypt("value",options_crypt_key)+") VALUES("+crypt->valueEncrypt("smtp_ssl",options_crypt_key)+","+
                                    crypt->valueEncrypt(QVariant(new_data->ssl).toString(),options_crypt_key)+");");
    }

    return result;
}

// **** EMAIL **** }}
