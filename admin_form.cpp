#include "admin_form.h"
#include "ui_admin_form.h"
#include "change_admin_pw_dialog.h"
#include "question_mod_dialog.h"

#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <QFile>

admin_form::admin_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin_form)
{
    ui->setupUi(this);

    this->setWindowTitle(QApplication::applicationName()+": admin mode");
    ui->treeWidget_students->hideColumn(1);
    ui->treeWidget_test_questions->hideColumn(1);
    ui->treeWidget_test_questions->hideColumn(2);
    connect(ui->treeWidget_test_questions,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(set_questions_buttons_availablity(QTreeWidgetItem*)));
    ui->treeWidget_learn_questions->hideColumn(1);
    ui->treeWidget_learn_questions->hideColumn(2);
    connect(ui->treeWidget_learn_questions,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(set_questions_buttons_availablity(QTreeWidgetItem*)));

    getDataBases();

    act_addGroup = new QAction(QIcon(":/stud/add_group"),tr("Add Group"),ui->toolButton_Add_Stud->menu());
    connect(act_addGroup,SIGNAL(triggered()),this,SLOT(on_actionAddGroup_triggered()));
    ui->toolButton_Add_Stud->addAction(act_addGroup);

    act_addStud = new QAction(QIcon(":/stud/add_stud"),tr("Add Student"),ui->toolButton_Add_Stud->menu());
    connect(act_addStud,SIGNAL(triggered()),this,SLOT(on_actionAddStud_triggered()));
    ui->toolButton_Add_Stud->addAction(act_addStud);

    act_addTheme = new QAction(tr("Add Theme"),ui->toolButton_Add_Quest->menu());
    connect(act_addTheme,SIGNAL(triggered()),this,SLOT(on_action_addTheme_triggered()));
    ui->toolButton_Add_Quest->addAction(act_addTheme);

    act_addQuest = new QAction(tr("Add Question"),ui->toolButton_Add_Quest->menu());
    ui->toolButton_Add_Quest->addAction(act_addQuest);

    sql = new sql_cl();
    setAvailabilityOfItems(sql->dbIsOpen());
}

admin_form::~admin_form()
{
    if(sql) delete sql;
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
    ui->pushButton_Edit_Quest->setEnabled(false);
    ui->pushButton_Del_Quest->setEnabled(false);
}
//
void admin_form::getDataBases()
{
    this->setCursor(Qt::BusyCursor);
    QDir db_dir(DBPath,
                "*.qlt",
                QDir::Name,
                QDir::Files|QDir::Readable|QDir::Writable|QDir::NoSymLinks);

    QStringList db_files =db_dir.entryList();
    ui->listWidget_DB->clear();

    for(int i=0; i < db_files.count(); i++){
        QString curFile = db_files.at(i);
        QFileInfo f_info(DBPath+curFile);

        if(f_info.isReadable() && f_info.isWritable()){
            ui->listWidget_DB->addItem(curFile.replace(".QLT","",Qt::CaseInsensitive));
        }
        else{
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Can\'t open file")+" \'"+DBPath+curFile+"\' "+tr("for read and write. Skipped."));
        }
    }
    this->setCursor(Qt::ArrowCursor);
}
//
// --- Database --- {{
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
sql = new sql_cl();
        QFileInfoList db_files = QDir(DBPath).entryInfoList(QStringList() << "*", QDir::Files);
        for(int i=0; i < db_files.count(); i++){
    if(db_files.at(i).baseName() == new_db_name){
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Name have been used already.\n Please, introduse new name."));
        break;
    }

    else {

        if(sql->openDB(DBPath+new_db_name+".QLT")){
            if(sql->createNewDB()){
                getDataBases();
          }

            else{
                QMessageBox::critical(this,
                                      tr("Error"),
                                      tr("Can't create database struct."));
           }
        }
        else{
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Error on create new database."));
         }
       }
     }
   }
 }
//
void admin_form::on_pushButton_DelDB_clicked()
{
    QListWidgetItem *curDB_item = ui->listWidget_DB->currentItem();
    if(curDB_item){
        int ret = QMessageBox::question(this, tr("Removing database"),
                                        tr("Are you shure want delete database")+" \""+curDB_item->text()+"\" ?",
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
        if(ret == QMessageBox::Yes){
            if(sql->dbIsOpen()) sql->closeDB();
            setAvailabilityOfItems(sql->dbIsOpen());
            QFile file_for_del(DBPath+curDB_item->text()+".QLT");

            if(file_for_del.remove()){
                getDataBases();
            }
            else{
                QMessageBox::critical(this,
                                      tr("Error"),
                                      file_for_del.errorString());
            }
        }
    }
}
//
void admin_form::on_listWidget_DB_clicked()
{
    QString db_file = ui->listWidget_DB->currentItem()->text().trimmed();
    bool db_is_open = sql->openDB(QApplication::applicationDirPath()+"/data/"+db_file+".QLT");
    setAvailabilityOfItems(db_is_open);
    if(db_is_open){
        getQuestionList();
        getQuestionList(0);
        getQuestionList(1);
        getStudentsList();
    }
    else{

    }
}
// --- Database --- }}
// --- tab questions --- {{
QTreeWidget* admin_form::get_curQTW(int q_type)
{
    QTreeWidget *result;
    int question_Type = q_type;

    if(q_type == -1){
        question_Type = (ui->tabWidget_Questions->currentIndex()<2) ? ui->tabWidget_Questions->currentIndex() : 0;
    }

    if(question_Type == 0){
        result = ui->treeWidget_test_questions;
    }
    else if(question_Type == 1){
        result = ui->treeWidget_learn_questions;
    }
    return result;
}
//
void admin_form::getQuestionList(int q_type)
{
    this->setCursor(Qt::BusyCursor);
    QTreeWidget *curQTW = get_curQTW(q_type);

    curQTW->clear();
    QList<st_svMAP> q_res = sql->getThemes(); // select themes from database
    QList<st_QTWI> tmp_ThemeList;
    if(q_res.count() > 0){
        st_QTWI newitem_data; // create struct for saving theme data
        for(int i = 0; i < q_res.count(); i++){
            QStringList newItem_sl;
            newItem_sl.append(q_res.at(i).map["NAME"].toString());
            newItem_sl.append(q_res.at(i).map["ID"].toString());
            newItem_sl.append("t"); // set mark

            newitem_data.parent_id = q_res.at(i).map["PARENT_ID"].toString().trimmed(); // save theme parent id
            newitem_data.qtwi = new QTreeWidgetItem(newItem_sl); // create theme item

            // add questions to theme
            QList<st_svMAP> themeQuestions = sql->getQuestions(q_res.at(i).map["ID"].toInt());
            if(themeQuestions.count()>0){ // if questions is found, then add founded questions to theme item
                for(int q=0;q<themeQuestions.count();q++){
                    QStringList newQuestion;
                    newQuestion.append(themeQuestions.at(q).map["QUESTION"].toString());
                    newQuestion.append(themeQuestions.at(q).map["ID"].toString());
                    newQuestion.append("q"); // set mark
                    newitem_data.qtwi->addChild(new QTreeWidgetItem(newQuestion));
                }
            }

            if(newitem_data.parent_id.trimmed().length() > 0){ // check for having parent for current theme
                QList<QTreeWidgetItem *> parentItems = curQTW->findItems(newitem_data.parent_id,Qt::MatchExactly,1); // if parent exists, find him in tree
                if(parentItems.count() > 0){
                    parentItems.at(0)->addChild(newitem_data.qtwi); // if parent found, add current theme item to parent
                }
                else{
                    tmp_ThemeList.append(newitem_data); // if parent not found, save current theme item to the temporery list
                }
            }
            else{
                curQTW->insertTopLevelItem(0,newitem_data.qtwi); // if theme without parent, add current theme item to the root of the tree
            }
        }
        if(tmp_ThemeList.count() > 0){ // if temporary list is not empty then
            for(int i=0;i<tmp_ThemeList.count();i++){
                QList<QTreeWidgetItem *> parentItems = curQTW->findItems(tmp_ThemeList.at(i).parent_id,Qt::MatchExactly,1); // find parent for theme item from list
                if(parentItems.count() > 0){
                    parentItems.at(0)->addChild(newitem_data.qtwi); // if found parent for theme item from list, then add this theme item to parent
                }
                else{
                    curQTW->insertTopLevelItem(0,newitem_data.qtwi); // if not found, then add theme item to the root of the tree
                }
            }
        }
    }
    curQTW->setSortingEnabled(true);
    curQTW->sortItems(0,Qt::AscendingOrder);
    this->setCursor(Qt::ArrowCursor);
}
//
void admin_form::prepareThemesDlg(theme_dlg *dlg, QTreeWidget *curQTW, QString exclude_id)
{
    QList<st_svMAP> q_res_themes = sql->getThemes();
    dlg->clear_PThemes();
    dlg->add_PTheme(tr("No parent"),"0");

    if(q_res_themes.count() > 0){

        for(int i = 0;i < q_res_themes.count(); i++){
            if(q_res_themes.at(i).map["ID"].toString() != exclude_id){
                dlg->add_PTheme(q_res_themes.at(i).map["NAME"].toString(),
                        q_res_themes.at(i).map["ID"].toString(),
                        q_res_themes.at(i).map["PARENT_ID"].toString());
            }
        }
    }

    if(curQTW->currentItem()->parent()){
        //        if(curQTW->currentItem()->text(2) == "t"){
        //            dlg->set_current_PTheme(curQTW->currentItem()->text(1));
        //        }
        //        else{
        dlg->set_current_PTheme(curQTW->currentItem()->parent()->text(1));
        //        }
    }
    else{
        dlg->set_current_PTheme("0");
    }
}
//
void admin_form::on_action_addTheme_triggered()
{
    QTreeWidget *curQTW = get_curQTW();

    // save last selection for Add button as "theme"
    ui->toolButton_Add_Quest->setText(tr("Add theme"));
    connect(ui->toolButton_Add_Quest,SIGNAL(clicked()),this,SLOT(on_action_addTheme_triggered()));

    theme_dlg th_dlg(this);
    th_dlg.setWindowTitle(tr("Add new theme"));

    prepareThemesDlg(&th_dlg,curQTW);

    QString new_themeName, PThemeID;
    if(th_dlg.exec() == 1){
        new_themeName = th_dlg.get_ThemeName();
        PThemeID = th_dlg.get_PThemeID();

        if(new_themeName.trimmed().length() > 0){
            bool q_result;
            if(PThemeID.trimmed().length() > 0 && QVariant(PThemeID).toInt() > 0){
                q_result = sql->addTheme(new_themeName,PThemeID);
            }
            else{
                q_result = sql->addTheme(new_themeName);
            }

            if(q_result) {
                getQuestionList(0);
                getQuestionList(1);
            }
        }
    }
}
//
void admin_form::on_toolButton_Add_Quest_clicked()
{
    if(ui->toolButton_Add_Quest->text() == tr("Add")) ui->toolButton_Add_Quest->showMenu();
}
//
void admin_form::on_pushButton_Edit_Quest_clicked()
{
    QTreeWidget *curQTW = get_curQTW();
    if(curQTW->currentItem()->text(2) == "t"){
        qDebug() << "edit theme: " << curQTW->currentItem()->text(0);
        theme_dlg th_dlg(this);
        th_dlg.setWindowTitle(tr("Edit theme"));

        prepareThemesDlg(&th_dlg,curQTW,curQTW->currentItem()->text(1).trimmed());
        th_dlg.set_current_ThemeName(curQTW->currentItem()->text(0).trimmed());

        if(th_dlg.exec() == 1){
            QString new_themeName, PThemeID, upd_data, q_str;
            new_themeName = th_dlg.get_ThemeName();
            PThemeID = th_dlg.get_PThemeID();
            upd_data.clear();
            if(new_themeName.trimmed().length() > 0){
                if(new_themeName.trimmed() != curQTW->currentItem()->text(0).trimmed()){
                    upd_data.append("name=\'"+new_themeName+"\'");
                }
                if(PThemeID.trimmed() != ((curQTW->currentItem()->parent()) ? curQTW->currentItem()->parent()->text(1).trimmed() : "0")){
                    if(upd_data.length() > 0) upd_data.append(", ");
                    upd_data.append("parent_id = "+PThemeID.trimmed());
                }

                if(upd_data.trimmed().length() > 0){
                    q_str ="UPDATE qthemes SET "+upd_data+" WHERE id="+curQTW->currentItem()->text(1).trimmed()+
                            " AND name=\'"+curQTW->currentItem()->text(0).trimmed()+"\' AND parent_id="+
                            ((curQTW->currentItem()->parent()) ? curQTW->currentItem()->parent()->text(1).trimmed() : "0")+";";
                }

                if(sql->SendSimpleQueryStr(q_str)){
                    getQuestionList(0);
                    getQuestionList(1);
                }
            }
        }
    }
    else{
        qDebug() << "edit question: " << curQTW->currentItem()->text(0);
    }

}
//
void admin_form::set_questions_buttons_availablity(QTreeWidgetItem *item)
{
    if(item){
        ui->pushButton_Edit_Quest->setEnabled(true);
        ui->pushButton_Del_Quest->setEnabled(true);
    }
    else{
        ui->pushButton_Edit_Quest->setEnabled(false);
        ui->pushButton_Del_Quest->setEnabled(false);
    }
}
// !!!! --- tab questions --- !!!! }}
// !!!! --- tab students --- !!!! {{
void admin_form::getStudentsList()
{
    QList<st_svMAP> q_res_groups, q_res_stud;
    q_res_groups.append(sql->getGroups());

    if(q_res_groups.count() > 0){
        QList<QTreeWidgetItem *> groups, students;

        QList<QString> group, student;
        ui->treeWidget_students->clear();
        for(int i = 0;i < q_res_groups.count(); i++){
            group.clear();
            students.clear();

            group.append(q_res_groups.at(i).map["CODE"].toString());
            group.append(q_res_groups.at(i).map["ID"].toString());

            q_res_stud.clear();
            q_res_stud = sql->getStudents(q_res_groups.at(i).map["ID"].toString());

            for(int s = 0;s < q_res_stud.count(); s++){
                student.clear();
                student.append(q_res_stud.at(s).map["SURNAME"].toString()+" "+
                        q_res_stud.at(s).map["NAME"].toString()+" "+
                        q_res_stud.at(s).map["PATRONYMIC"].toString());
                student.append(q_res_stud.at(s).map["ID"].toString());
                QTreeWidgetItem *stud_item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(student));
                stud_item->setIcon(0,QIcon(":/stud/stud"));
                students.append(stud_item);
            }
            QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(group));
            item->setIcon(0,QIcon(":/stud/group"));
            item->addChildren(students);
            groups.append(item);
        }
        ui->treeWidget_students->insertTopLevelItems(0,groups);
        ui->treeWidget_students->setSortingEnabled(true);
        ui->treeWidget_students->sortByColumn(0,Qt::AscendingOrder);
    }
}
//
void admin_form::on_treeWidget_students_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
    if(curItem){
        QMenu *menu=new QMenu(this);
        if(curItem->parent()){
            QAction *act_EditStud = new QAction(QIcon(":/stud/edit_stud"),tr("Edit student"), this);
            connect(act_EditStud,SIGNAL(triggered()),this,SLOT(on_pushButton_Edit_Stud_clicked()));
            QAction *act_DelStud = new QAction(QIcon(":/stud/del_stud"),tr("Delete student"),this);
            connect(act_DelStud,SIGNAL(triggered()),this,SLOT(on_pushButton_Delete_Stud_clicked()));
            menu->addAction(act_addStud);
            menu->addAction(act_EditStud);
            menu->addSeparator();
            menu->addAction(act_DelStud);
        }
        else{
            QAction *act_EditGroup = new QAction(QIcon(":/stud/edit_group"),tr("Edit group"), this);
            connect(act_EditGroup,SIGNAL(triggered()),this,SLOT(on_pushButton_Edit_Stud_clicked()));
            QAction *act_ClearGroup = new QAction(QIcon(":/resourse/erase"),tr("Clear group"),this);
            connect(act_ClearGroup,SIGNAL(triggered()),this,SLOT(on_action_clearGroup_clicked()));
            QAction *act_DelGroup = new QAction(QIcon(":/stud/del_group"),tr("Delete group"),this);
            connect(act_DelGroup,SIGNAL(triggered()),this,SLOT(on_pushButton_Delete_Stud_clicked()));
            menu->addAction(act_addStud);
            menu->addSeparator();
            menu->addAction(act_addGroup);
            menu->addAction(act_EditGroup);
            menu->addSeparator();
            menu->addAction(act_ClearGroup);
            menu->addAction(act_DelGroup);
        }

        menu->popup(ui->treeWidget_students->viewport()->mapToGlobal(pos));
    }
}
//
void admin_form::on_toolButton_Add_Stud_clicked()
{
    if(ui->toolButton_Add_Stud->text() == tr("Add")) ui->toolButton_Add_Stud->showMenu();
}
//
void admin_form::on_actionAddGroup_triggered(QString group_code)
{
    QString new_grpName;
    if(group_code.isEmpty()){
        // save last selection for Add button as "group"
        ui->toolButton_Add_Stud->setText(act_addGroup->text());
        ui->toolButton_Add_Stud->setIcon(QIcon(":/stud/add_group"));
        connect(ui->toolButton_Add_Stud,SIGNAL(clicked()),this,SLOT(on_actionAddGroup_triggered()));

        new_grpName = QInputDialog::getText(this,tr("Input group name"),QObject::tr("Please name of new group")).trimmed();
    }
    else{
        new_grpName = group_code.trimmed();
    }

    if(new_grpName.trimmed().length() > 0){
        bool res = sql->addGroup(new_grpName);
        if(res) getStudentsList();
    }
}
//
bool admin_form::prepareAddStudDlg(stud_dlg *dlg)
{
    QList<st_svMAP> q_res_groups = sql->getGroups();
    bool result;
    if(q_res_groups.count() > 0){
        dlg->comboBox_groups_clear();
        for(int i = 0;i < q_res_groups.count(); i++){
            dlg->comboBox_groups_addItem(q_res_groups.at(i).map["CODE"].toString(),
                    q_res_groups.at(i).map["ID"]);
        }

        QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
        if(!curItem->parent()){
            dlg->comboBox_groups_set_curItem(curItem->text(0));
        }
        else {
            dlg->comboBox_groups_set_curItem(curItem->parent()->text(0));
        }
        result = true;
    }
    else{
        result = false;
    }
    return result;
}
//
void admin_form::on_actionAddStud_triggered()
{
    // save last selection for Add button as "student"
    ui->toolButton_Add_Stud->setText(act_addStud->text());
    ui->toolButton_Add_Stud->setIcon(QIcon(":/stud/add_stud"));
    connect(ui->toolButton_Add_Stud,SIGNAL(clicked()),this,SLOT(on_actionAddStud_triggered()));


    stud_dlg dlg(this);
    dlg.setWindowTitle(tr("Add new student"));

    if(prepareAddStudDlg(&dlg)){
        if(dlg.exec() == 1){
            if(sql->studUnique(dlg.get_lineEdit_Surname().trimmed(),
                              dlg.get_lineEdit_Name().trimmed(),
                              dlg.get_lineEdit_Patronymic().trimmed(),
                              dlg.get_group_id().toString())){

                st_stud new_stud;
                new_stud.grp_code = dlg.get_group_code();
                new_stud.name = dlg.get_lineEdit_Name();
                new_stud.surname = dlg.get_lineEdit_Surname();
                new_stud.patronymic = dlg.get_lineEdit_Patronymic();

                if(sql->addStudent(new_stud)){
                    getStudentsList();
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
                if(sql->grpUnique(in_grp)){
                    bool q_res = sql->SendSimpleQueryStr("UPDATE GROUPS SET CODE=\'"+in_grp+"\' WHERE CODE=\'"+
                                                    curItem->text(0)+"\' AND ID="+curItem->text(1)+";");
                    if(q_res){
                        getStudentsList();
                    }
                }
            }

        }
        else{ // edit student
            st_qRes q_res = sql->getStudent(curItem->text(1).trimmed(),curItem->parent()->text(1).trimmed());

            if(q_res.q_result){
                stud_dlg dlg(this);
                dlg.setWindowTitle(tr("Edit student"));

                if(prepareAddStudDlg(&dlg)){
                    dlg.lineEdit_Name_setText(q_res.sel_data.at(0).map["NAME"].toString());
                    dlg.lineEdit_Surname_setText(q_res.sel_data.at(0).map["SURNAME"].toString());
                    dlg.lineEdit_Patronymic_setText(q_res.sel_data.at(0).map["PATRONYMIC"].toString());

                    if(dlg.exec() == 1){
                        QString updFields;
                        updFields.clear();
                        if(dlg.get_group_id() != q_res.sel_data.at(0).map["GROUP_ID"]){
                            updFields.append("GROUP_ID="+dlg.get_group_id().toString());
                        };

                        if(dlg.get_lineEdit_Name() != q_res.sel_data.at(0).map["NAME"].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append("NAME=\'"+dlg.get_lineEdit_Name()+"\'");
                        }

                        if(dlg.get_lineEdit_Surname() != q_res.sel_data.at(0).map["SURNAME"].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append("SURNAME=\'"+dlg.get_lineEdit_Surname()+"\'");
                        }

                        if(dlg.get_lineEdit_Patronymic() != q_res.sel_data.at(0).map["PATRONYMIC"].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append("PATRONYMIC=\'"+dlg.get_lineEdit_Patronymic()+"\'");
                        }

                        if(updFields.length() > 0){
                            if(sql->SendSimpleQueryStr("UPDATE students SET "+updFields+" WHERE id="+curItem->text(1)+
                                                  " AND group_id="+curItem->parent()->text(1)+";")){
                                getStudentsList();
                            }
                        }
                    }
                }
            }
        }
    }
}
//
void admin_form::on_action_clearGroup_clicked()
{
    QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
    if(curItem->parent()){
        QMessageBox::warning(this,tr("Error"),tr("Please select group for clean."));
    }
    else{
        sql->clearGroup(curItem->text(1));
        getStudentsList();
    }
}

//
void admin_form::on_pushButton_Delete_Stud_clicked()
{
    QTreeWidgetItem *curItem = ui->treeWidget_students->currentItem();
    if(curItem->parent()){
        // student
        int ret = QMessageBox::question(this, tr("Removing student"),
                                        tr("Are you shure want delete student")+" \n\""+
                                        curItem->text(0).trimmed()+"\" \n"+
                                        tr("from group")+" \""+curItem->parent()->text(0).trimmed()+"\" ?",
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
        if(ret == QMessageBox::Yes){
            if(sql->delStudent(curItem->text(1).trimmed(),curItem->parent()->text(1).trimmed())){
                getStudentsList();
            }
        }
    }
    else{
        //group
        int ret = QMessageBox::question(this, tr("Removing group"),
                                        tr("Are you shure want delete group")+" \n\""+
                                        curItem->text(0).trimmed()+"\" \n "+
                                        tr("and all students included to this group")+"?",
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
        if(ret == QMessageBox::Yes){
            if(sql->clearGroup(curItem->text(1))){
                sql->delGroup(curItem->text(1));
            }
            getStudentsList();
        }
    }
}
//
bool admin_form::sendStudData_toDB(QList<st_stud> *data)
{
    bool result = true;
    for(int i=0; i<data->count();i++){
        if(sql->grpUnique(data->at(i).grp_code,true)){
            sql->addGroup(data->at(i).grp_code);
        }
        if(sql->studUnique(data->at(i).surname,
                          data->at(i).name,
                          data->at(i).patronymic,
                          sql->getGroupIdByCode(data->at(i).grp_code).toString(),true)){
            result = sql->addStudent(data->at(i));
        }

        if(!result) break;
    }
    data->clear();
    return result;
}

//
void admin_form::on_pushButton_Import_Stud_clicked()
{
    QList<st_stud> impData;

    QFile file(QFileDialog::getOpenFileName(this, tr("Open files"),"/home/", "(*.csv)"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList strings;
        QTextStream in(&file);
        const int step = 10;
        while (!in.atEnd()) {
            st_stud new_stud;
            strings = in.readLine().split(";");

            new_stud.grp_code = strings.at(0).trimmed();
            new_stud.surname = strings.at(1).trimmed();
            new_stud.name = strings.at(2).trimmed();
            new_stud.patronymic = strings.at(3).trimmed();

            impData.append(new_stud);
            if(impData.count() >= step){
                sendStudData_toDB(&impData);
            }
        }
        file.close();
        if(impData.count() > 0){
            sendStudData_toDB(&impData);
        }
        getStudentsList();
    }
    else
    {
        qDebug() << "Error open for read";
        QMessageBox::critical(this,"Error open", "Don't open files!");
    }
    qDebug() << "Impdata.count: " << impData.count();

}
//
void admin_form::on_treeWidget_students_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()){
        ui->pushButton_Edit_Stud->setIcon(QIcon(":/stud/edit_stud"));
        ui->pushButton_Delete_Stud->setIcon(QIcon(":/stud/del_stud"));
    }
    else{
        ui->pushButton_Edit_Stud->setIcon(QIcon(":/stud/edit_group"));
        ui->pushButton_Delete_Stud->setIcon(QIcon(":/stud/del_group"));
    }
}
//  !!!! --- tab students --- !!!! }}

