#include "dbfunc.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
//**********************************************
qResult SendSimpleQueryStr(QSqlDatabase *db,const QString& q_str)
{
    qResult result;
    QSqlQuery *query = new QSqlQuery(*db);

    db->transaction();
    if(!query->exec(q_str)){
        result.query_result = false;
        result.text = "query: "+q_str+"\ndriver: "+query->lastError().driverText()+
                "\ndatabase: "+query->lastError().databaseText();
        db->rollback();
        qDebug() << query->lastError().databaseText();
        qDebug() << query->lastError().driverText();
    }
    else{
        result.query_result = true;
        db->commit();
    }
    query->clear();
    delete query;

    return result;
}
//**********************************************
qResult SendSimpleQueryStrWR(QSqlDatabase *db,const QString& q_str)
{
    qResult result;
    st_svMAP col;

    QSqlQuery *query = new QSqlQuery(*db);

    if(!query->exec(q_str)){
        result.query_result = false;
        result.text = "query: "+q_str+"\ndriver: "+query->lastError().driverText()+
                "\ndatabase: "+query->lastError().databaseText();
        db->rollback();
        qDebug() << query->lastError().databaseText();
        qDebug() << query->lastError().driverText();
    }
    else{
        result.query_result = true;
        result.selection_result.clear();
        st_svMAP col;
        while(query->next()){
            for(int counter=0; counter < query->record().count() ;counter++){
                col.map.insert(query->record().fieldName(counter),query->value(counter));
            }
            result.selection_result << col;
            col.map.clear();
        }
    }
    query->clear();
    return result;
}
