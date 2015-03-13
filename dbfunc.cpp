#include "dbfunc.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
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
bool addGroup(QSqlDatabase *db, QString grpCode)
{
    bool result;
    if(grpUnique(db,grpCode)){
        result = SendSimpleQueryStr(db,"INSERT INTO GROUPS(CODE) VALUES('"+grpCode+"');");
    }
    else{
        result = false;
    }
    return result;
}
//
bool clearGroup(QSqlDatabase *db, const QVariant grpId)
{
    return SendSimpleQueryStr(db,"DELETE FROM STUDENTS WHERE GROUP_ID="+grpId.toString());
}
//
bool delGroup(QSqlDatabase *db, const QVariant grpId)
{
    return SendSimpleQueryStr(db,"DELETE FROM GROUPS WHERE ID="+grpId.toString());
}

//
bool grpUnique(QSqlDatabase *db, const QString grpCode, bool silent)
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
bool addStudent(QSqlDatabase *db,st_stud data)
{
    return SendSimpleQueryStr(db,"INSERT INTO STUDENTS(GROUP_ID,NAME,SURENAME,PATRONIMYC) VALUES("+
                              getGroupIdByCode(db,data.grp_code).toString()+",\'"+data.name+"\',\'"+
                              data.surename+"\',\'"+data.patronymic+"\');");
}

//
bool delStudent(QSqlDatabase *db, QString studId, QString grpId)
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
bool studUnique(QSqlDatabase *db, const QString Surename, const QString Name, const QString Patrinymic, QString grpId, bool silent)
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
                                         "\' AND SURENAME=\'"+Surename+"\' AND PATRONIMYC=\'"+Patrinymic+"\'"+
                                         cond_grpId+";");
    if(q_res.q_result){
        if(q_res.sel_data.at(0).map["STUD_EXISTS"].toInt() == 0){
            result = true;
        }
        else{
            result = false;
            if(!silent){
                QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("Student")+" \""+Surename+" "+Name+" "+Patrinymic+
                                      "\" "+QObject::tr("already exists")+msg_grpId+"!");
            }
        }
    }
    return result;
}

// **** STUDENTS **** }}
