#include "dbfunc.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

//**********************************************
bool openDB(QSqlDatabase *db,QString db_file)
{
    bool result;
    if(db->isOpen()) db->close();
    db->setDatabaseName(db_file);
    db->setUserName("SYSDBA");
    //    db->setPassword("masterkey");
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

    st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT BLANK FROM OPTIONS;");
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
        result = SendSimpleQueryStr(db,"UPDATE OPTIONS SET BLANK=0;");
    }

    return result;
}
//**********************************************
// **** QUESTIONS **** {{
bool sql_ThemeUnique(QSqlDatabase *db, const QString themeName, bool silent)
{
    bool result;
    if(themeName.isEmpty() || themeName.isNull() || themeName.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT ID FROM QTHEMES WHERE NAME='"+themeName+"';");
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
QList<st_svMAP> getThemeByID(QSqlDatabase *db, QVariant theme_id)
{
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                              FROM qthemes \
                                              WHERE ID="+theme_id.toString()+
                                              " ORDER BY qthemes.id");
    return result.sel_data;

}
//
QList<st_svMAP> getThemes(QSqlDatabase *db)
{
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                              FROM qthemes \
                                              ORDER BY qthemes.id");
    return result.sel_data;
}

//
bool sql_addTheme(QSqlDatabase *db, const QString themeName, QString parent_id)
{
    bool result = false;
    result = sql_ThemeUnique(db,themeName.trimmed());
    if(result){
        QString q_str = "INSERT INTO QTHEMES(";
        if(parent_id > 0) q_str.append("PARENT_ID,");
        q_str.append("NAME) VALUES(");
        if(parent_id > 0) q_str.append(parent_id+",");
        q_str.append("\'"+themeName.trimmed()+"\');");
        result = SendSimpleQueryStr(db,q_str);
    }
    return result;
}
//
QList<st_svMAP> sql_getQuestionsWithThemes(QSqlDatabase *db, int questions_type)
{
    QString _questions_type;
    if(questions_type == 0){
        _questions_type = "TEST";
    }
    else{
        _questions_type = "LEARN";
    }
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT * \
                                              FROM VW_"+_questions_type+"_QUESTIONS_WITH_THEMES;");
    return result.sel_data;
}

//
QList<st_svMAP> sql_getQuestions(QSqlDatabase *db,QString theme_id)
{
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                              FROM qthemes \
                                              ORDER BY qthemes.id");
    return result.sel_data;
}

// **** QUESTIONS **** }}
//**********************************************
// **** GROUPS **** {{
QList<st_svMAP> getGroups(QSqlDatabase *db){
    st_qRes result = SendSimpleQueryStrWR(db,"SELECT * FROM GROUPS");
    return result.sel_data;
}
//
QString getGroupCodeById(QSqlDatabase *db,QString grpId)
{
    QString result;
    st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT CODE FROM GROUPS WHERE ID="+grpId.trimmed());
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
    st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT ID FROM GROUPS WHERE CODE=\'"+grpCode.trimmed()+"\';");
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
        result = SendSimpleQueryStr(db,"INSERT INTO GROUPS(CODE) VALUES('"+grpCode+"');");
    }
    else{
        result = false;
    }
    return result;
}
//
bool sql_clearGroup(QSqlDatabase *db, const QVariant grpId)
{
    return SendSimpleQueryStr(db,"DELETE FROM STUDENTS WHERE GROUP_ID="+grpId.toString());
}
//
bool sql_delGroup(QSqlDatabase *db, const QVariant grpId)
{
    return SendSimpleQueryStr(db,"DELETE FROM GROUPS WHERE ID="+grpId.toString());
}

//
bool sql_grpUnique(QSqlDatabase *db, const QString grpCode, bool silent)
{
    bool result;
    if(grpCode.isEmpty() || grpCode.isNull() || grpCode.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR(db,"SELECT CODE FROM GROUPS WHERE CODE='"+grpCode+"';");
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
    result = SendSimpleQueryStrWR(db, "SELECT * FROM STUDENTS"+condition+";");

    return result.sel_data;
}
//
st_qRes getStudent(QSqlDatabase *db, QString studId, QString groupID)
{
    return SendSimpleQueryStrWR(db, "SELECT * FROM STUDENTS WHERE ID="+studId+
                                " AND GROUP_ID="+groupID+";");
}
//
bool sql_addStudent(QSqlDatabase *db,st_stud data)
{
    return SendSimpleQueryStr(db,"INSERT INTO STUDENTS(GROUP_ID,NAME,SURNAME,PATRONYMIC) VALUES("+
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
        cond_grpId = " AND GROUP_ID="+grpId;
    }
    return SendSimpleQueryStr(db,"DELETE FROM STUDENTS WHERE ID="+studId+cond_grpId+";");
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
        cond_grpId = " AND GROUP_ID="+grpId;
        msg_grpId =  QObject::tr(" in group")+" \""+getGroupCodeById(db,grpId)+"\"";
    }

    st_qRes q_res = SendSimpleQueryStrWR(db,
                                         "SELECT count(*) AS STUD_EXISTS FROM STUDENTS WHERE NAME=\'"+Name+
                                         "\' AND SURNAME=\'"+Surname+"\' AND PATRONYMIC=\'"+Patrinymic+"\'"+
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
