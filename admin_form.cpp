#include "admin_form.h"
#include "ui_admin_form.h"
#include "change_admin_pw_dialog.h"
#include "question_mod_dialog.h"
#include "dbfunc.h"

#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>


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
    qResult q_groups_res = SendSimpleQueryStrWR(&db,"SELECT * FROM GROUPS");
    qResult q_stud_res;
    if(q_groups_res.query_result){
        QList<QTreeWidgetItem *> groups, students;

        QList<QString> group, student;
        ui->treeWidget_students->clear();
        for(int i = 0;i < q_groups_res.selection_result.count(); i++){
            group.clear();
            students.clear();
            q_stud_res.query_result = false;
            group.append(q_groups_res.selection_result.at(i).map["CODE"].toString());
            group.append(q_groups_res.selection_result.at(i).map["ID"].toString());

            q_stud_res = SendSimpleQueryStrWR(&db,
                                              "SELECT * FROM STUDENTS WHERE GROUP_ID="+
                                              q_groups_res.selection_result.at(i).map["ID"].toString()+
                    ";");
            if(q_stud_res.query_result){
                for(int s = 0;s < q_stud_res.selection_result.count(); s++){
                    student.append(q_stud_res.selection_result.at(s).map["SURENAME"].toString()+" "+
                            q_stud_res.selection_result.at(s).map["NAME"].toString()+" "+
                            q_stud_res.selection_result.at(s).map["PATRONIMYC"].toString());
                    student.append(q_stud_res.selection_result.at(s).map["ID"].toString());
                    students.append(new QTreeWidgetItem((QTreeWidget*)0,
                                                        QStringList(student)));
                }
            }
            else{
                QMessageBox::critical(this,tr("Error"),q_stud_res.text);
            }
            QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(group));
            item->addChildren(students);
            groups.append(item);

        }
        ui->treeWidget_students->insertTopLevelItems(0,groups);
    }
    else{
        QMessageBox::critical(this,tr("Error"),q_groups_res.text);
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
bool admin_form::prepareAddStudDlg(add_stud_dlg *dlg)
{
    qResult q_res = SendSimpleQueryStrWR(&db,"SELECT * FROM GROUPS;");
    if(!q_res.query_result){
        QMessageBox::critical(this,tr("Error"),q_res.text);
    }
    else{
        dlg->comboBox_groups_clear();
        for(int i = 0;i < q_res.selection_result.count(); i++){
            dlg->comboBox_groups_addItem(q_res.selection_result.at(i).map["CODE"].toString(),
                    q_res.selection_result.at(i).map["ID"]);
        }

        QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
        if(!curItem->parent()){
            dlg->comboBox_groups_set_curItem(curItem->text(0));
        }
        else {
            dlg->comboBox_groups_set_curItem(curItem->parent()->text(0));
        }
    }
    return q_res.query_result;
}
//
void admin_form::on_actionAddStud_triggered()
{
    // save last selection for Add button as "student"
    ui->toolButton_Add_Stud->setText(addStud->text());
    connect(ui->toolButton_Add_Stud,SIGNAL(clicked()),this,SLOT(on_actionAddStud_triggered()));

    add_stud_dlg dlg(this);
    dlg.setWindowTitle(tr("Add new student"));

    if(prepareAddStudDlg(&dlg)){
        if(dlg.exec() == 1){
            qResult q_res = SendSimpleQueryStrWR(&db,
                                                 "SELECT count(*) AS STUD_EXISTS FROM STUDENTS WHERE NAME=\'"+
                                                 dlg.get_lineEdit_Name()+"\' AND SURENAME=\'"+
                                                 dlg.get_lineEdit_Surename()+"\' AND PATRONIMYC=\'"+
                                                 dlg.get_lineEdit_Patronymic()+"\' AND GROUP_ID="+
                                                 dlg.get_group_id().toString()+";");
            if(!q_res.query_result){
                QMessageBox::critical(this,tr("Error"),q_res.text);
            }
            else{
                if(q_res.selection_result.at(0).map["STUD_EXISTS"].toInt() > 0){
                    QMessageBox::critical(this,tr("Error"),
                                          tr("Student")+" \""+dlg.get_lineEdit_Surename()+" "+
                                          dlg.get_lineEdit_Name()+" "+dlg.get_lineEdit_Patronymic()+
                                          "\" "+tr("already exists in group")+" \""+dlg.get_group_code()+"\"");
                }
                else{
                    q_res = SendSimpleQueryStr(&db,
                                               "INSERT INTO STUDENTS(GROUP_ID,NAME,SURENAME,PATRONIMYC) VALUES("+
                                               dlg.get_group_id().toString()+",\'"+dlg.get_lineEdit_Name()+"\',\'"+
                                               dlg.get_lineEdit_Surename()+"\',\'"+dlg.get_lineEdit_Patronymic()+"\');");
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
                                                 "SELECT * FROM STUDENTS WHERE ID="+curItem->text(1)+
                                                 " AND GROUP_ID="+curItem->parent()->text(1)+";");
            if(!q_res.query_result){
                QMessageBox::critical(this,tr("Error"),q_res.text);
            }
            else{
                add_stud_dlg dlg(this);
                dlg.setWindowTitle(tr("Edit student"));

                if(prepareAddStudDlg(&dlg)){
                    dlg.lineEdit_Name_setText(q_res.selection_result.at(0).map["NAME"].toString());
                    dlg.lineEdit_Surename_setText(q_res.selection_result.at(0).map["SURENAME"].toString());
                    dlg.lineEdit_Patronymic_setText(q_res.selection_result.at(0).map["PATRONIMYC"].toString());
//                    studData.stud.stud_id = QVariant(curItem->text(1)).toInt();

                    if(dlg.exec() == 1){
                        QString updFields;
                        updFields.clear();
                        if(dlg.get_group_id() != q_res.selection_result.at(0).map["GROUP_ID"]){
                            updFields.append("GROUP_ID="+dlg.get_group_id().toString());
                        };

                        if(dlg.get_lineEdit_Name() != q_res.selection_result.at(0).map["NAME"].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append("NAME=\'"+dlg.get_lineEdit_Name()+"\'");
                        }

                        if(dlg.get_lineEdit_Surename() != q_res.selection_result.at(0).map["SURENAME"].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append("SURENAME=\'"+dlg.get_lineEdit_Surename()+"\'");
                        }

                        if(dlg.get_lineEdit_Patronymic() != q_res.selection_result.at(0).map["PATRONIMYC"].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append("PATRONIMYC=\'"+dlg.get_lineEdit_Patronymic()+"\'");
                        }

                        if(updFields.length() > 0){
                            q_res = SendSimpleQueryStr(&db,"UPDATE STUDENTS SET "+updFields+" WHERE ID="+curItem->text(1)+
                                                       " AND GROUP_ID="+curItem->parent()->text(1)+";");
                            if(!q_res.query_result)QMessageBox::critical(this,tr("Error"),q_res.text);

                        }
                    }
                }
            }
        }
    }
}
//
void admin_form::on_treeWidget_students_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
    if(curItem){
        QMenu *menu=new QMenu(this);
        if(!curItem->parent()){
            QAction *act_EditGroup = new QAction(tr("Edit group"), this);
            connect(act_EditGroup,SIGNAL(triggered()),this,SLOT(on_pushButton_Edit_Stud_clicked()));
            QAction *act_ClearGroup = new QAction(tr("Clear group"),this);
            QAction *act_DelGroup = new QAction(tr("Delete group"),this);
            menu->addAction(addStud);
            menu->addSeparator();
            menu->addAction(addGroup);
            menu->addAction(act_EditGroup);
            menu->addSeparator();
            menu->addAction(act_ClearGroup);
            menu->addAction(act_DelGroup);
        }
        else{
            QAction *act_EditStud = new QAction(tr("Edit student"), this);
            connect(act_EditStud,SIGNAL(triggered()),this,SLOT(on_pushButton_Edit_Stud_clicked()));
            QAction *act_DelStud = new QAction(tr("Delete student"),this);
            menu->addAction(addStud);
            menu->addAction(act_EditStud);
            menu->addSeparator();
            menu->addAction(act_DelStud);
        }
        menu->popup(ui->treeWidget_students->viewport()->mapToGlobal(pos));
    }
}
// --- tab students --- }}




