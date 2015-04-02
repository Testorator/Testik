#include "dbfunc.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

//**********************************************
bool createNewDB()
{

}
//
bool openDB(QSqlDatabase *db,QString db_file)
{
    bool result;
    if(db->isOpen()) db->close();
    db->setDatabaseName(db_file);
    db->setUserName("SYSDBA");
    //       db->setPassword("masterkey");
    db->setPassword("XGn8#w!H");

    db->open();
    result = db->isOpen();

    if(!result){
        QMessageBox msgBox;
        msgBox.setText("DB file serror: "+db_file);
        msgBox.setDetailedText("Driver: "+db->lastError().driverText()+
                               "\nDatabase: "+db->lastError().databaseText());
        msgBox.exec();
    }
    else{
        qDebug() << "DB openinig - success";
    }
    return result;
}
//**********************************************
bool SendSimpleQueryStr(QSqlDatabase *db,const QString& q_str)
{
    bool result;
    QSqlQuery *query = new QSqlQuery(*db);

    db->transaction();
    if(!query->exec(q_str)){
        result = false;
        db->rollback();
        qDebug() << query->lastError().databaseText();
        qDebug() << query->lastError().driverText();
        QMessageBox::critical(new QWidget,QObject::tr("Error"),"query: "+q_str+"\ndriver: "+
                              query->lastError().driverText()+
                              "\ndatabase: "+query->lastError().databaseText());
    }
    else{
        result = true;
        db->commit();
    }
    query->clear();
    delete query;

    return result;
}
//**********************************************
QString convrtTypeOfQuestions(int type)
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
st_qRes SendSimpleQueryStrWR(QSqlDatabase *db,const QString& q_str)
{
    st_qRes result;

    result.sel_data.clear();
    st_svMAP col;

    QSqlQuery *query = new QSqlQuery(*db);

    if(!query->exec(q_str)){
        db->rollback();
        result.q_result = false;
        qDebug() << query->lastError().databaseText();
        qDebug() << query->lastError().driverText();
        QMessageBox::critical(new QWidget,QObject::tr("Error"),"query: "+q_str+"\ndriver: "+
                              query->lastError().driverText()+
                              "\ndatabase: "+query->lastError().databaseText());
    }
    else{
        result.q_result = true;
        st_svMAP col;
        while(query->next()){
            for(int counter=0; counter < query->record().count() ;counter++){
                col.map.insert(query->record().fieldName(counter),query->value(counter));
            }
            result.sel_data << col;
            col.map.clear();
        }
    }
    query->clear();
    return result;
}
//**********************************************
bool isBlankDB(QSqlDatabase *db, QString db_file)
{
    bool result = false;
    if(!db_file.isEmpty() && !db_file.isNull() && db_file.trimmed().length()>0){
        openDB(db,db_file);
    }

    st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT blank FROM options;");
    if(q_res.q_result){
        result = q_res.sel_data.at(0).map["BLANK"].toBool();
    }

    if(!db_file.isEmpty() && !db_file.isNull() && db_file.trimmed().length()>0){
        db->close();
    }

    return result;
}
//
bool setDB_NoBlank(QSqlDatabase *db,QString db_file)
{
    bool result = false;

    result = openDB(db,db_file);
    if(result){
        result = SendSimpleQueryStr(db,"UPDATE options SET blank=0;");
    }

    return result;
}
//**********************************************
// **** QUESTIONS **** {{
bool sql_themeUnique(QSqlDatabase *db, const QString themeName, bool silent)
{
    bool result;
    if(themeName.isEmpty() || themeName.isNull() || themeName.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT id FROM qthemes WHERE name='"+themeName+"';");
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
QList<st_svMAP> sql_getThemeByID(QSqlDatabase *db, QVariant theme_id)
{
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                          FROM qthemes \
                                          WHERE ID="+theme_id.toString()+
                                                   " ORDER BY qthemes.id");
    return result.sel_data;

}
//
QList<st_svMAP> sql_getThemes(QSqlDatabase *db)
{
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                          FROM qthemes ORDER BY qthemes.id");
                                          return result.sel_data;
}

//
bool sql_addTheme(QSqlDatabase *db, const QString themeName, QString parent_id)
{
    bool result = false;
    result = sql_themeUnique(db,themeName.trimmed());
    if(result){
        QString q_str = "INSERT INTO qthemes(";
        if(parent_id > 0) q_str.append("parent_id,");
        q_str.append("name) VALUES(");
        if(parent_id > 0) q_str.append(parent_id+",");
        q_str.append("\'"+themeName.trimmed()+"\');");
        result = SendSimpleQueryStr(db,q_str);
    }
    return result;
}
//
QList<st_svMAP> sql_getThemeChild(QSqlDatabase *db, QVariant parent_id)
{
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT * FROM qthemes WHERE parent_id="+parent_id.toString()+";");
    return result.sel_data;
}
//
QList<st_svMAP> sql_getQuestionsWithThemes(QSqlDatabase *db, int questions_type)
{
    QString _questions_type = convrtTypeOfQuestions(questions_type);

    st_qRes result = SendSimpleQueryStrWR(db,"SELECT * \
                                          FROM vw_"+_questions_type+"_questions_with_themes;");
            return result.sel_data;
}

//
QList<st_svMAP> sql_getQuestions(QSqlDatabase *db, int questions_type, QString theme_id)
{
    QString _questions_type = convrtTypeOfQuestions(questions_type);
    QString condition = "";
    if(theme_id.trimmed().length()>0){
        condition = "WHERE theme_id = "+theme_id.trimmed()+" ";
    }

    st_qRes result = SendSimpleQueryStrWR(db,"SELECT vw_"+_questions_type+"_questions.id,vw_"+_questions_type+
                                          "_questions.theme_id, vw_"+_questions_type+"_questions.question \
                                          FROM vw_"+_questions_type+"_questions "+condition+
                                                                                "order by vw_"+_questions_type+"_questions.id, \
                                          vw_"+_questions_type+"_questions.question ");
                                          return result.sel_data;
}

// **** QUESTIONS **** }}
//**********************************************
// **** GROUPS **** {{
QList<st_svMAP> getGroups(QSqlDatabase *db){
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT * FROM groups");
    return result.sel_data;
}
//
QString getGroupCodeById(QSqlDatabase *db,QString grpId)
{
    QString result;
    st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT code FROM groups WHERE id="+grpId.trimmed());
    if(q_res.q_result){
        result = q_res.sel_data.at(0).map["CODE"].toString();
    }
    else{
        result.clear();
    }
    return result;
}
//
QVariant getGroupIdByCode(QSqlDatabase *db,QString grpCode)
{
    QVariant result;
    st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT id FROM groups WHERE code=\'"+grpCode.trimmed()+"\';");
    if(q_res.q_result){
        result = q_res.sel_data.at(0).map["ID"];
    }
    else{
        result.clear();
    }
    return result;
}
//
bool sql_addGroup(QSqlDatabase *db, QString grpCode)
{
    bool result;
    if(sql_grpUnique(db,grpCode)){
        result = SendSimpleQueryStr(db,"INSERT INTO groups(code) VALUES('"+grpCode+"');");
    }
    else{
        result = false;
    }
    return result;
}
//
bool sql_clearGroup(QSqlDatabase *db, const QVariant grpId)
{
    return SendSimpleQueryStr(db,"DELETE FROM students WHERE group_id="+grpId.toString());
}
//
bool sql_delGroup(QSqlDatabase *db, const QVariant grpId)
{
    return SendSimpleQueryStr(db,"DELETE FROM groups WHERE id="+grpId.toString());
}

//
bool sql_grpUnique(QSqlDatabase *db, const QString grpCode, bool silent)
{
    bool result;
    if(grpCode.isEmpty() || grpCode.isNull() || grpCode.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT code FROM groups WHERE code='"+grpCode+"';");
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
QList<st_svMAP> getStudents(QSqlDatabase *db,QString groupID)
{
    st_qRes result;
    QString condition;
    if(groupID.isEmpty() || groupID.isNull() || groupID.trimmed().length() < 1){
        condition.clear();
    }
    else{
        condition = " WHERE GROUP_ID="+groupID.trimmed()+" ";
    }
    result = SendSimpleQueryStrWR(db, "SELECT * FROM students"+condition+";");

    return result.sel_data;
}
//
st_qRes sql_getStudent(QSqlDatabase *db, QString studId, QString groupID)
{
    return SendSimpleQueryStrWR(db, "SELECT * FROM students WHERE id="+studId+
                                " AND group_id="+groupID+";");
}
//
bool sql_addStudent(QSqlDatabase *db,st_stud data)
{
    return SendSimpleQueryStr(db,"INSERT INTO students(group_id,name,surname,patronymic) VALUES("+
                              getGroupIdByCode(db,data.grp_code).toString()+",\'"+data.name+"\',\'"+
                              data.surname+"\',\'"+data.patronymic+"\');");
}

//
bool sql_delStudent(QSqlDatabase *db, QString studId, QString grpId)
{
    QString cond_grpId;
    if(grpId.isEmpty() || grpId.isNull() || grpId.trimmed().length() < 1){
        cond_grpId.clear();
    }
    else{
        cond_grpId = " AND group_id="+grpId;
    }
    return SendSimpleQueryStr(db,"DELETE FROM students WHERE id="+studId+cond_grpId+";");
}
//
bool sql_studUnique(QSqlDatabase *db, const QString Surname, const QString Name, const QString Patrinymic, QString grpId, bool silent)
{
    bool result = false;
    QString cond_grpId, msg_grpId;
    if(grpId.isEmpty() || grpId.isNull() || grpId.trimmed().length() < 1){
        cond_grpId.clear();
        msg_grpId.clear();
    }
    else{
        cond_grpId = " AND group_id="+grpId;
        msg_grpId =  QObject::tr(" in group")+" \""+getGroupCodeById(db,grpId)+"\"";
    }

    st_qRes q_res = SendSimpleQueryStrWR(db,
                                         "SELECT count(*) AS stud_exists FROM students WHERE name=\'"+Name+
                                         "\' AND surname=\'"+Surname+"\' AND patronymic=\'"+Patrinymic+"\'"+
                                         cond_grpId+";");
    if(q_res.q_result){
        if(q_res.sel_data.at(0).map["STUD_EXISTS"].toInt() == 0){
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
