#include "dbfunc.h"
#include "encryption.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QApplication>
#include <QTableView>

sql_cl::sql_cl(QSqlDatabase &db)
{
    cur_db = QSqlDatabase(db);
}
//
sql_cl::~sql_cl()
{
    if(cur_db.isOpen()) cur_db.close();
}

//**********************************************
bool sql_cl::createNewDB()
{

    QSqlQuery create;
    QString stud = "CREATE TABLE students (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "group_id INTEGER,"
            "name TEXT NOT NULL,"
            "surname TEXT NOT NULL,"
            "patronymic TEXT,"
            "FOREIGN KEY (group_id) REFERENCES groups (id) );";
    bool st = create.exec(stud);
    if(!st){
        qDebug() << create.lastError();
    }
    QString quest = "CREATE TABLE questions (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
            "theme_id INTEGER NOT NULL, "
            "for_learn INTEGER NOT NULL DEFAULT 0, "
            "question TEXT NOT NULL, "
            "FOREIGN KEY (theme_id) REFERENCES q_themes (id) );";

   bool que = create.exec(quest);
   if(!que){


       qDebug() << create.lastError();
   }
    QString theme = "CREATE TABLE q_themes ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
            "parent_id INTEGER NOT NULL DEFAULT 0, "
            "name TEXT NOT NULL UNIQUE, "
            "FOREIGN KEY (parent_id) REFERENCES q_themes (id) );";
    bool th = create.exec(theme);
    if(!th){
        qDebug() << create.lastError();
    }
     QString opt = "CREATE TABLE options (send_report_by_email INTEGER NOT NULL DEFAULT 0);";
     bool op = create.exec(opt);
             if(!op) {
                 qDebug() << create.lastError();
             }
     QString grp = "CREATE TABLE groups (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
             "Code TEXT NOT NULL UNIQUE);";
     bool gr = create.exec(grp);
     if(!gr) {
     qDebug() << create.lastError();
     }
     QString answer = "CREATE TABLE answers (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
             "question_id INTEGER NOT NULL, "
             "correct INTEGER NOT NULL DEFAULT 0, "
             "answer TEXT, "
             "comment TEXT, "
             "FOREIGN KEY (question_id) REFERENCES question (id) );";
             bool ans = create.exec(answer);
     if(!ans) {
         qDebug() << create.lastError();

     }

     QString email = "CREATE TABLE email_addreses (recipient_name TEXT NOT NULL, "
             "address TEXT NOT NULL );";
     bool mail = create.exec(answer);
if(!mail) {
 qDebug() << create.lastError();

}

create.exec("CREATE VIEW vw_test_questions (AS SELECT id, theme_id, question\
                FROM question\
                WHERE for_learn = 0 );");



create.exec("CREATE VIEW vw_learn_questions (AS SELECT id, theme_id, question\
                        FROM question\
                        WHERE for_learn > 0 );");

}
//
bool sql_cl::openDB(QString db_file)
{
    bool result;
    if(cur_db.isOpen()) cur_db.close();
    cur_db.setDatabaseName(db_file);
    cur_db.open();
    result = cur_db.isOpen();

    if(!result){
        QMessageBox msgBox;
        msgBox.setText("DB file serror: "+db_file);
        msgBox.setDetailedText("Driver: "+cur_db.lastError().driverText()+
                               "\nDatabase: "+cur_db.lastError().databaseText());
        msgBox.exec();
    }
    else{
        qDebug() << "DB openinig - success";
    }
    return result;
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
st_qRes sql_cl::SendSimpleQueryStrWR(const QString& q_str)
{
    st_qRes result;

    result.sel_data.clear();
    st_svMAP col;

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
// **** QUESTIONS **** {{
bool sql_cl::themeUnique(const QString themeName, bool silent)
{
    bool result;
    if(themeName.isEmpty() || themeName.isNull() || themeName.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT id FROM qthemes WHERE name='"+themeName+"';");
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
QList<st_svMAP> sql_cl::getThemeByID(QVariant theme_id)
{
    st_qRes result = SendSimpleQueryStrWR("SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                          FROM qthemes \
                                          WHERE ID="+theme_id.toString()+
                                                   " ORDER BY qthemes.id");
    return result.sel_data;

}
//
QList<st_svMAP> sql_cl::getThemes()
{
    st_qRes result = SendSimpleQueryStrWR("SELECT qthemes.id, qthemes.parent_id, qthemes.name \
                                          FROM qthemes ORDER BY qthemes.id");
                                          return result.sel_data;
}

//
bool sql_cl::addTheme(const QString themeName, QString parent_id)
{
    bool result = false;
    result = themeUnique(themeName.trimmed());
    if(result){
        QString q_str = "INSERT INTO qthemes(";
        if(parent_id > 0) q_str.append("parent_id,");
        q_str.append("name) VALUES(");
        if(parent_id > 0) q_str.append(parent_id+",");
        q_str.append("\'"+themeName.trimmed()+"\');");
        result = SendSimpleQueryStr(q_str);
    }
    return result;
}
//
QList<st_svMAP> sql_cl::getThemeChild(QVariant parent_id)
{
    st_qRes result = SendSimpleQueryStrWR("SELECT * FROM qthemes WHERE parent_id="+parent_id.toString()+";");
    return result.sel_data;
}
//
QList<st_svMAP> sql_cl::getQuestionsWithThemes(int questions_type)
{
    QString _questions_type = convertTypeOfQuestions(questions_type);

    st_qRes result = SendSimpleQueryStrWR("SELECT * \
                                          FROM vw_"+_questions_type+"_questions_with_themes;");
            return result.sel_data;
}

//
QList<st_svMAP> sql_cl::getQuestions(int questions_type, QString theme_id)
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
QList<st_svMAP> sql_cl::getGroups(){
    st_qRes result = SendSimpleQueryStrWR("SELECT * FROM groups");
    return result.sel_data;
}
//
QString sql_cl::getGroupCodeById(QString grpId)
{
    QString result;
    st_qRes q_res = SendSimpleQueryStrWR("SELECT code FROM groups WHERE id="+grpId.trimmed());
    if(q_res.q_result){
        result = q_res.sel_data.at(0).map["CODE"].toString();
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
    st_qRes q_res = SendSimpleQueryStrWR("SELECT id FROM groups WHERE code=\'"+grpCode.trimmed()+"\';");
    if(q_res.q_result){
        result = q_res.sel_data.at(0).map["ID"];
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
        result = SendSimpleQueryStr("INSERT INTO groups(code) VALUES('"+grpCode+"');");
    }
    else{
        result = false;
    }
    return result;
}
//
bool sql_cl::clearGroup(const QVariant grpId)
{
    return SendSimpleQueryStr("DELETE FROM students WHERE group_id="+grpId.toString());
}
//
bool sql_cl::delGroup(const QVariant grpId)
{
    return SendSimpleQueryStr("DELETE FROM groups WHERE id="+grpId.toString());
}

//
bool sql_cl::grpUnique(const QString grpCode, bool silent)
{
    bool result;
    if(grpCode.isEmpty() || grpCode.isNull() || grpCode.trimmed().length() < 1){
        result =false;
    }
    else{
        st_qRes q_res = SendSimpleQueryStrWR("SELECT code FROM groups WHERE code='"+grpCode+"';");
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
QList<st_svMAP> sql_cl::getStudents(QString groupID)
{
    st_qRes result;
    QString condition;
    if(groupID.isEmpty() || groupID.isNull() || groupID.trimmed().length() < 1){
        condition.clear();
    }
    else{
        condition = " WHERE GROUP_ID="+groupID.trimmed()+" ";
    }
    result = SendSimpleQueryStrWR("SELECT * FROM students"+condition+";");

    return result.sel_data;
}
//
st_qRes sql_cl::getStudent(QString studId, QString groupID)
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
