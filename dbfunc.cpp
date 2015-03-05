#include "dbfunc.h"
#include <QSqlQuery>

//**********************************************
bool SendSimpleQueryStr(QSqlDatabase *db,const QString& q_str)
{
    bool result;
    QSqlQuery *query = new QSqlQuery(*db);

    db->transaction();
    if(!query->exec(q_str)){
        result = false;
        db->rollback();
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
//QList<st_svMAP> wwdb_cl::SendSimpleQueryStrWR(const QString& owner,const QString& q_str)
//{
//    QList<st_svMAP> result;
//    st_svMAP col;

//    genDebug(owner+" (wwdb - SendSimpleQueryWR)",q_str);

//    if(!query->exec(q_str)){
//        genErrReport(owner+" (wwdb - SendSimpleQueryWR)",q_str);
//    }
//    else{
//        while(query->next()){
//            for(counter=0; counter < query->record().count() ;counter++){
//                col.map.insert(query->record().fieldName(counter),query->value(counter));
//            }
//            result << col;
//            col.map.clear();
//        }
//    }
//    query->clear();
//    return result;
//}
