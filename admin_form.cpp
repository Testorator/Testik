#include "admin_form.h"
#include "ui_admin_form.h"
#include "change_admin_pw_dialog.h"
#include "question_mod_dialog.h"
#include "dbfunc.h"
#include "add_stud_dlg.h"

#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>


admin_form::admin_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin_form)
{
    ui->setupUi(this);
    this->setWindowTitle(QApplication::applicationName()+": admin mode");
    ui->treeWidget_students->hideColumn(1);
    getDataBases();
    db = QSqlDatabase::addDatabase("QIBASE");

    addGroup = new QAction(tr("Add Group"),ui->toolButton_Add_Stud->menu());
    connect(addGroup,SIGNAL(triggered()),this,SLOT(on_actionAddGroup_triggered()));
    ui->toolButton_Add_Stud->addAction(addGroup);

    addStud = new QAction(tr("Add Student"),ui->toolButton_Add_Stud->menu());
    connect(addStud,SIGNAL(triggered()),this,SLOT(on_actionAddStud_triggered()));
    ui->toolButton_Add_Stud->addAction(addStud);

    setAvailabilityOfItems(db.isOpen());
}

admin_form::~admin_form()
{
    if(db.isOpen()) db.close();
    delete ui;
}

//
void admin_form::on_actionChange_admin_password_triggered()
{
    change_admin_pw_dialog chPW_dialog;
    chPW_dialog.exec();
}
//
void admin_form::setAvailabilityOfItems(bool val)
{
    ui->groupBox_SendEMail->setEnabled(val);
    ui->tabWidget->setEnabled(val);
}
//
void admin_form::on_pushButton_clicked()
{
    question_mod_dialog qmd;
    qmd.exec();
}
//
void admin_form::getDataBases()
{
    QDir db_dir(QApplication::applicationDirPath()+"/data/",
                "*.qlt",
                QDir::Name,
                QDir::Files|QDir::Readable|QDir::Writable|QDir::NoSymLinks);

    QStringList db_files = db_dir.entryList();
    db_files.removeAt(db_files.indexOf("BLANK.QLT"));

    ui->listWidget_DB->clear();
    ui->listWidget_DB->addItems(db_files);
}
//
void admin_form::on_pushButton_AddDB_clicked()
{
    QString new_db_name = QInputDialog::getText(this,
                                                QObject::tr("Input database name"),
                                                QObject::tr("Please input name for new database"));

    if(new_db_name.trimmed().length() == 0){
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("The name of the new database is empty!\nOperaion cancelled."));
    }
    else{
        if(QFile::copy(QApplication::applicationDirPath()+"/data/BLANK.QLT",
                       QApplication::applicationDirPath()+"/data/"+new_db_name+".QLT")){
            getDataBases();
        }
        else{
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Error on create new database."));
        }
    }
}
//
void admin_form::on_listWidget_DB_clicked()
{
    QString db_file = ui->listWidget_DB->currentItem()->text().trimmed();
    if(db.isOpen()) db.close();
    db.setDatabaseName(QApplication::applicationDirPath()+"/data/"+db_file);
    db.setUserName("SYSDBA");
    db.setPassword("XGn8#w!H");
    db.open();
    setAvailabilityOfItems(db.isOpen());
    if(db.isOpen()){
        qDebug() << "DB openinig - success";
        getStudentsList();
    }
    else{
        QMessageBox::critical(this,
                              tr("Error"),
                              "database: "+QApplication::applicationDirPath()+"/data/"+db_file+"\n"+db.lastError().text());
    }
}
// --- tab students --- {{
void admin_form::getStudentsList()
{
    qResult q_res = SendSimpleQueryStrWR(&db,"SELECT * FROM GROUPS");
    if(q_res.query_result){
        QList<QTreeWidgetItem *> items;
        QList<QString> item;
        ui->treeWidget_students->clear();
        for(int i = 0;i < q_res.selection_result.count(); i++){
            item.clear();
            item.append(q_res.selection_result.at(i).map["CODE"].toString());
            item.append(q_res.selection_result.at(i).map["ID"].toString());
            items.append(new QTreeWidgetItem((QTreeWidget*)0,
                                             QStringList(item)
                                             ));

        }
        ui->treeWidget_students->insertTopLevelItems(0,items);
    }
}
//
void admin_form::on_toolButton_Add_Stud_clicked()
{
    if(ui->toolButton_Add_Stud->text() == tr("Add")) ui->toolButton_Add_Stud->showMenu();
}
//
void admin_form::on_actionAddGroup_triggered()
{
    // save last selection for Add button as "group"
    ui->toolButton_Add_Stud->setText(addGroup->text());
    connect(ui->toolButton_Add_Stud,SIGNAL(clicked()),this,SLOT(on_actionAddGroup_triggered()));

    qDebug() << "Add group clicked";
    QString in_grp = QInputDialog::getText(this,
                                           tr("Input group name"),
                                           QObject::tr("Please name of new group"));
    if(in_grp.trimmed().length() > 0){
        qResult q_res = SendSimpleQueryStrWR(&db,"SELECT CODE FROM GROUPS WHERE CODE='"+in_grp.trimmed()+"';");
        if(!q_res.query_result){
            QMessageBox::critical(this,tr("Error"),q_res.text);
        }
        else{
            if(q_res.selection_result.count() > 0){
                QMessageBox::critical(this,tr("Error"),tr("Group with name ")+"\""+in_grp.trimmed()+"\""+tr(" already exists!"));
            }
            else{
                q_res = SendSimpleQueryStr(&db,"INSERT INTO GROUPS(CODE) VALUES('"+in_grp.trimmed()+"');");
                if(!q_res.query_result){
                    QMessageBox::critical(this,tr("Error"),q_res.text);
                }
                else{
                    getStudentsList();
                }
            }
        }
    }
}
//
void admin_form::on_actionAddStud_triggered()
{
    // save last selection for Add button as "student"
    ui->toolButton_Add_Stud->setText(addStud->text());
    connect(ui->toolButton_Add_Stud,SIGNAL(clicked()),this,SLOT(on_actionAddStud_triggered()));

    //    QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
    //    if(curItem){
    //        if(curItem->childCount() > 0){ // edit group
    //            studData.grp_code = curItem->text(0);
    //            studData.grp_id = QVariant(curItem->text(1)).toInt();
    //            QString in_grp = QInputDialog::getText(this,
    //                                                   tr("Modification group name"),
    //                                                   tr("Please enter new name for group")+" \""+studData.grp_code+"\"",
    //                                                   QLineEdit::Normal,
    //                                                   studData.grp_code);
    //            qDebug() << "new name=" << in_grp;
    //        }
    //    }


}
//
void admin_form::on_pushButton_Edit_Stud_clicked()
{
    QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
    if(!curItem){
        QMessageBox::information(this,
                                 tr("Information"),
                                 tr("Please select group or student for modification"));
    }
    else{
        stud_data studData;

        if(!curItem->parent()){ // edit group
            QString in_grp = QInputDialog::getText(this,
                                                   tr("Modification group name"),
                                                   tr("Please enter new name for group")+" \""+curItem->text(0)+"\"",
                                                   QLineEdit::Normal,
                                                   curItem->text(0)).trimmed();
            if(in_grp.length() > 0){
                // check for unique
                qResult q_res = SendSimpleQueryStrWR(&db,"SELECT * FROM GROUPS WHERE CODE=\'"+in_grp+"\';");
                if(!q_res.query_result){
                    QMessageBox::critical(this,tr("Error"),q_res.text);
                }
                else{
                    if(q_res.selection_result.count() > 0){
                        QMessageBox::critical(this,
                                              tr("Error"),
                                              tr("Group with name")+" \""+in_grp+"\" "+tr("already exists!"));
                    }
                    else{
                        qResult q_res = SendSimpleQueryStr(&db,
                                                           "UPDATE GROUPS SET CODE=\'"+in_grp+"\' WHERE CODE=\'"
                                                           +curItem->text(0)+"\' AND ID="+curItem->text(1)+";");
                        if(!q_res.query_result){
                            QMessageBox::critical(this,tr("Error"),q_res.text);
                        }
                        else{
                            getStudentsList();
                        }
                    }
                }
            }
        }
        else{ // edit student
            qResult q_res = SendSimpleQueryStrWR(&db,
                                                 "SELECT * FROM GROUPS WHERE ID="+curItem->text(1)+
                                                 " AND GROUP_ID="+curItem->parent()->text(1)+";");
            if(!q_res.query_result){
                QMessageBox::critical(this,tr("Error"),q_res.text);
            }
            else{
                studData.grp_code = curItem->parent()->text(0);
                studData.grp_id = QVariant(curItem->parent()->text(1)).toInt();
                studData.stud_id = QVariant(curItem->text(1)).toInt();
                studData.stud_name = q_res.selection_result.at(0).map["NAME"].toString();
                studData.stud_patronymic = q_res.selection_result.at(0).map["PATRONIMYC"].toString();
                studData.stud_surename = q_res.selection_result.at(0).map["SURENAME"].toString();
                add_stud_dlg dlg(&studData,this);
                if(dlg.exec() == 1){
                    qDebug() << "grp_id: " << studData.grp_id << " grp_code: " << studData.grp_code;
                }
            }
        }
    }
}
// --- tab students --- }}


