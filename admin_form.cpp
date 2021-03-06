#include "admin_form.h"
#include "ui_admin_form.h"
#include "change_admin_pw_dialog.h"
#include "test_mod_dlg.h"
#include "email.h"
#include "email_dlg.h"
#include "smtp_set.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <QFile>
#include <QToolBar>
#include <QHeaderView>

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
    connect(act_addQuest, SIGNAL(triggered()),this,SLOT(on_action_addQuest_triggered()));
    ui->toolButton_Add_Quest->addAction(act_addQuest);

    QToolBar* p_email_tb = new QToolBar("EMail toolbar");
    p_email_tb->setIconSize(QSize(18, 18));
    addAddr = new QAction(QIcon(":/resource/add"),tr("Add"),p_email_tb);
    connect(this->addAddr,SIGNAL(triggered()),this,SLOT(on_action_addAddr_triggered()));
    editAddr = new QAction(QIcon(":/resource/edit"),tr("Edit"),p_email_tb);
    connect(this->editAddr,SIGNAL(triggered()),this,SLOT(on_action_editAddr_triggered()));
    delAddr = new QAction(QIcon(":/resource/erase"),tr("Delete"),p_email_tb);
    connect(this->delAddr,SIGNAL(triggered()),this,SLOT(on_action_delAddr_trigered()));
    SMTP_settings = new QAction(QIcon(":/resource/options"),tr("SMTP settings"),p_email_tb);
    connect(this->SMTP_settings,SIGNAL(triggered()),this,SLOT(on_action_SMTP_settings_triggered()));
    p_email_tb->addAction(addAddr);
    p_email_tb->addSeparator();
    p_email_tb->addAction(editAddr);
    p_email_tb->addSeparator();
    p_email_tb->addAction(delAddr);
    p_email_tb->addSeparator();
    p_email_tb->addAction(SMTP_settings);

    ui->gridLayout_email->addWidget(p_email_tb,0,0,0,0,Qt::AlignTop);

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

    QStringList db_files = db_dir.entryList();
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
void admin_form::loadQuestions(QTreeWidget *curQTW, int q_type)
{
    curQTW->clear();
    QList<st_theme> q_res = sql->getThemes(); // select themes from database
    QList<st_QTWI> tmp_ThemeList;
    if(q_res.count() > 0){
        st_QTWI newitem_data; // create struct for saving theme data
        for(int i = 0; i < q_res.count(); i++){
            QStringList newItem_sl;
            newItem_sl.append(q_res.at(i).name);
            newItem_sl.append(q_res.at(i).id);
            newItem_sl.append("t"); // set mark
            newitem_data.parent_id = (q_res.at(i).parent_id.toInt() != 0) ? q_res.at(i).parent_id.trimmed(): ""; // save theme parent id
            newitem_data.qtwi = new QTreeWidgetItem(newItem_sl); // create theme item

            // add questions to theme
            QList<st_question> themeQuestions = sql->getQuestions(q_type,q_res.at(i).id);
            if(themeQuestions.count()>0){ // if questions is found, then add founded questions to theme item
                for(int q=0;q<themeQuestions.count();q++){
                    QStringList newQuestion;
                    newQuestion.append(themeQuestions.at(q).text);
                    newQuestion.append(themeQuestions.at(q).id);
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
        bool new_db_ok = true;
        QFileInfoList db_files = QDir(DBPath).entryInfoList(QStringList() << "*", QDir::Files);
        for(int i=0; i < db_files.count(); i++){

            if(db_files.at(i).baseName() == new_db_name){
                QMessageBox::critical(this,
                                      tr("Error"),
                                      tr("Name have been used already.\n Please, introduse new name."));
                new_db_ok = false;
                break;
            }
        }

        if(new_db_ok){
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
        getQuestionList(0);
        getQuestionList(1);
        getStudentsList();
        getEMailAddrList();
        ui->groupBox_SendEMail->setChecked(sql->sendEMail());
    }
    else{
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Can't open database")+" \""+db_file+".qlt\"");
    }
}
// --- Database --- }}
// --- tab questions --- {{
int admin_form::get_curQTW_index()
{
    return (ui->tabWidget_Questions->currentIndex()<2) ? ui->tabWidget_Questions->currentIndex() : 0;
}

//
QTreeWidget* admin_form::get_curQTW(int q_type)
{
    QTreeWidget *result;
    int question_Type = q_type;

    if(q_type == -1){
        question_Type = get_curQTW_index();
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
    loadQuestions(curQTW,q_type);
    this->setCursor(Qt::ArrowCursor);
}
//
void admin_form::prepareThemesDlg(theme_dlg *dlg, QTreeWidget *curQTW, QString exclude_id)
{
    QList<st_theme> q_res_themes = sql->getThemes();
    dlg->clear_PThemes();
    dlg->add_PTheme(tr("."),"0");

    if(q_res_themes.count() > 0){

        for(int i = 0;i < q_res_themes.count(); i++){
            if(q_res_themes.at(i).id != exclude_id){
                dlg->add_PTheme(q_res_themes.at(i).name,q_res_themes.at(i).id,q_res_themes.at(i).parent_id);
            }
        }
    }

    if(curQTW->currentItem()){
        if(curQTW->currentItem()->text(2) == "t"){
            dlg->set_current_PTheme(curQTW->currentItem()->text(1));
        }
        else{
            if(curQTW->currentItem()->parent()){
                dlg->set_current_PTheme(curQTW->currentItem()->parent()->text(1));
            }
            else{
                dlg->set_current_PTheme("0");
            }
        }
    }
    else{
        dlg->set_current_PTheme("0");
    }
}
//
void admin_form::on_action_addTheme_triggered()
{
    QTreeWidget *curQTW = get_curQTW();

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
            st_theme new_data;
            bool q_result;
            new_data.name = new_themeName;
            if(PThemeID.trimmed().length() > 0 && QVariant(PThemeID).toInt() > 0){
                new_data.parent_id = PThemeID;
            }
            else{
                new_data.parent_id = "0";
            }

            q_result = sql->addTheme(&new_data);
            if(q_result) {
                getQuestionList(get_curQTW_index());
            }
        }
    }
}
//
//
void admin_form::on_pushButton_Del_Quest_clicked()
{
    QTreeWidget *curQTW = get_curQTW();
    QTreeWidgetItem *curItem = curQTW->currentItem();

    int ret = QMessageBox::question(this, tr("Removing theme"),
                                    tr("Are you shure want delete theme")+" \n\""+
                                    curItem->text(0).trimmed()+"\" \n "+
                                    tr("and all questions included to this theme")+"?",
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if(ret == QMessageBox::Yes){
        if(sql->clearTheme(curItem->text(1))){
            sql->delTheme(curItem->text(1));
        }
        getQuestionList(0);
        getQuestionList(1);
    }

}
//
void admin_form::on_toolButton_Add_Quest_clicked()
{
    if(ui->toolButton_Add_Quest->text() == tr("Add"))ui->toolButton_Add_Quest->showMenu();
}
//
void admin_form::prepareQuestDlg(question_mod_dialog *dlg)
{
    QList<st_theme> q_res_themes = sql->getThemes();

    if(q_res_themes.count() > 0){
        for(int i = 0;i < q_res_themes.count(); i++){
            dlg->addThemeToList(q_res_themes.at(i).name,q_res_themes.at(i).id);
        }
    }
}
//
void admin_form::on_action_addQuest_triggered()
{
    ui->toolButton_Add_Quest->setText(tr("Add question"));
    connect(ui->toolButton_Add_Quest,SIGNAL(clicked()),this,SLOT(on_action_addQuest_triggered()));
    question_mod_dialog *queMD_dialog = new question_mod_dialog();
    queMD_dialog->setWindowTitle(tr("Add question"));
    prepareQuestDlg(queMD_dialog);
    QTreeWidget *curQTW = get_curQTW();
    if(curQTW->currentItem()){
        queMD_dialog->setCurrentTheme(curQTW->currentItem()->text(1));
    }

    bool success = false;
    if(queMD_dialog->exec()){
        QString quest_text = queMD_dialog->getQuestionText();
        QString for_learn = QVariant(ui->tabWidget_Questions->currentIndex()).toString();
        QString comment = queMD_dialog->getQuestionComment();

        if(sql->addQuest(quest_text,for_learn,queMD_dialog->getQuestionTheme().toString(), queMD_dialog->getAnswersType().toString(), comment)){
            QVariant q_id = sql->getQuestIdByNameAndType(quest_text,for_learn);
            QList<st_answer> newAnswers = queMD_dialog->getAnswers();
            for(int i=0; i<newAnswers.count(); i++){
                st_answer new_answer = newAnswers.at(i);
                new_answer.question_id = q_id.toString();
                success = sql->addAnswer(&new_answer);
            }
        }
    }
    delete queMD_dialog;
    if(success){
        getQuestionList(get_curQTW_index());
    }
}
////
void admin_form::on_pushButton_Edit_Quest_clicked()
{
    QTreeWidget *curQTW = get_curQTW();
    bool success = false;
    if(curQTW->currentItem()->text(2) == "t"){
        theme_dlg *th_dlg = new theme_dlg();
        th_dlg->setWindowTitle(tr("Edit theme"));

        prepareThemesDlg(th_dlg,curQTW,curQTW->currentItem()->text(1).trimmed());
        th_dlg->set_current_ThemeName(curQTW->currentItem()->text(0).trimmed());

        if(th_dlg->exec() == 1){
            QString new_themeName, PThemeID, upd_data;
            new_themeName = th_dlg->get_ThemeName();
            PThemeID = th_dlg->get_PThemeID();
            upd_data.clear();
            if(new_themeName.trimmed().length() > 0){
                st_theme new_data;
                new_data.id = curQTW->currentItem()->text(1).trimmed();
                new_data.name = new_themeName.trimmed();
                new_data.parent_id = (curQTW->currentItem()->parent()) ? curQTW->currentItem()->parent()->text(1).trimmed() : "0";
                success = sql->updTheme(&new_data);
            }
        }
        delete th_dlg;
        if(success){
            getQuestionList(0);
            getQuestionList(1);
        }
    }
    else{
        question_mod_dialog *queMD_dialog = new question_mod_dialog();
        queMD_dialog->setWindowTitle(tr("Edit question"));
        prepareQuestDlg(queMD_dialog);
        QTreeWidget *curQTW = get_curQTW();
        st_question question_from_db;
        QList<st_answer> answers_from_db;
        if(curQTW->currentItem()){
            question_from_db = sql->getQuestionById(curQTW->currentItem()->text(1));
            queMD_dialog->setQuestionText(question_from_db.text);
            queMD_dialog->setCurrentTheme(question_from_db.theme_id);
            queMD_dialog->setQuestionComment(question_from_db.comment);
            queMD_dialog->setAnswersType(question_from_db.ans_type);

            answers_from_db.clear();
            answers_from_db = sql->getAnswers(curQTW->currentItem()->text(1).trimmed());
            queMD_dialog->loadAnswers(&answers_from_db);

        }
        if(queMD_dialog->exec()){
            st_question new_question_data = queMD_dialog->getQuestionData();
            new_question_data.id = question_from_db.id;
            success = sql->updateQuestion(&new_question_data);

            st_updAnswers new_answers_data = queMD_dialog->getAnswers4Update();
            QVariant q_id = sql->getQuestIdByNameAndType(queMD_dialog->getQuestionText(),QVariant(ui->tabWidget_Questions->currentIndex()).toString());

            for(int i=0; i < new_answers_data.answers4remove.count(); i++){
                st_answer answer4del;
                answer4del.ans_id = new_answers_data.answers4remove.at(i);
                answer4del.question_id = q_id.toString();
                sql->delAnswer(&answer4del);
            }

            for(int i=0; i < new_answers_data.answers4update.count(); i++){
                st_answer curAnswer = new_answers_data.answers4update.at(i);
                if(curAnswer.question_id.trimmed().length() == 0){
                    curAnswer.question_id = q_id.toString();
                }
                if(curAnswer.ans_id.trimmed().length()>0){
                    sql->updateAnswer(&curAnswer);
                }
                else{
                    sql->addAnswer(&curAnswer);
                }
            }
        }
        delete queMD_dialog;
        if(success){
            getQuestionList(0);
            getQuestionList(1);
        }
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
    QList<st_student> q_res_stud;

    QList<st_group> q_res_groups = sql->getGroups();

    if(q_res_groups.count() > 0){
        QList<QTreeWidgetItem *> groups, students;

        QList<QString> group, student;
        ui->treeWidget_students->clear();
        for(int i = 0;i < q_res_groups.count(); i++){
            group.clear();
            students.clear();

            group.append(q_res_groups.at(i).code);
            group.append(q_res_groups.at(i).id);

            q_res_stud.clear();
            q_res_stud = sql->getStudents(q_res_groups.at(i).id);

            for(int s = 0;s < q_res_stud.count(); s++){
                student.clear();
                student.append(q_res_stud.at(s).surname+" "+
                               q_res_stud.at(s).name+" "+
                               q_res_stud.at(s).patronymic);
                student.append(q_res_stud.at(s).id);
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
            QAction *act_ClearGroup = new QAction(QIcon(":/resource/erase"),tr("Clear group"),this);
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
    QList<st_group> q_res_groups = sql->getGroups();
    bool result;
    if(q_res_groups.count() > 0){
        dlg->comboBox_groups_clear();
        for(int i = 0;i < q_res_groups.count(); i++){
            dlg->comboBox_groups_addItem(q_res_groups.at(i).code,q_res_groups.at(i).id);
        }

        if(ui->treeWidget_students->currentItem() && ui->treeWidget_students->currentItem()->parent()){
            dlg->comboBox_groups_set_curItem(ui->treeWidget_students->currentItem()->parent()->text(0));
        }
        else {
            dlg->comboBox_groups_set_curItem(ui->treeWidget_students->currentItem()->text(0));
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
            if(sql->uniqStudent(dlg.get_lineEdit_Surname().trimmed(),
                                dlg.get_lineEdit_Name().trimmed(),
                                dlg.get_lineEdit_Patronymic().trimmed(),
                                dlg.get_group_id().toString())){

                st_student new_stud;
                new_stud.group.code = dlg.get_group_code();
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
                if(sql->uniqGroup(in_grp)){
                    bool q_res = sql->SendSimpleQueryStr(" "+sql->crypt->mdEncrypt("groups",sql->groups_crypt_key)+
                                                         " SET "+sql->crypt->mdEncrypt("code",sql->groups_crypt_key)+"="+
                                                         sql->crypt->valueEncrypt(in_grp,sql->groups_crypt_key)+" WHERE "+
                                                         sql->crypt->mdEncrypt("code",sql->groups_crypt_key)+
                                                         "="+sql->crypt->valueEncrypt(curItem->text(0),sql->groups_crypt_key)+
                                                         " AND "+sql->crypt->mdEncrypt("id",sql->groups_crypt_key)+"="+
                                                         curItem->text(1)+";");
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
                    dlg.lineEdit_Name_setText(q_res.sel_data.at(0)["name"].toString());
                    dlg.lineEdit_Surname_setText(q_res.sel_data.at(0)["surname"].toString());
                    dlg.lineEdit_Patronymic_setText(q_res.sel_data.at(0)["patronymic"].toString());

                    if(dlg.exec() == 1){
                        QString updFields,new_val,enc_fname;
                        updFields.clear();

                        enc_fname = sql->crypt->mdEncrypt("group_id",sql->students_crypt_key);
                        if(dlg.get_group_id() != q_res.sel_data.at(0)[enc_fname]){
                            updFields.append(enc_fname+"="+dlg.get_group_id().toString());
                        };

                        new_val = sql->crypt->valueEncrypt(dlg.get_lineEdit_Name(),sql->students_crypt_key);
                        enc_fname = sql->crypt->mdEncrypt("name",sql->students_crypt_key);
                        if(new_val != q_res.sel_data.at(0)[enc_fname].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append(sql->crypt->mdEncrypt("name",sql->students_crypt_key)+"="+new_val);
                        }

                        new_val = sql->crypt->valueEncrypt(dlg.get_lineEdit_Surname(),sql->students_crypt_key);
                        enc_fname = sql->crypt->mdEncrypt("surname",sql->students_crypt_key);
                        if(new_val != q_res.sel_data.at(0)[enc_fname].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append(enc_fname+"="+new_val);
                        }

                        new_val = sql->crypt->valueEncrypt(dlg.get_lineEdit_Patronymic(),sql->students_crypt_key);
                        enc_fname = sql->crypt->mdEncrypt("patronymic",sql->students_crypt_key);
                        if(new_val != q_res.sel_data.at(0)[enc_fname].toString()){
                            if(updFields.length() > 0) updFields.append(",");
                            updFields.append(enc_fname+"="+new_val);
                        }

                        if(updFields.length() > 0){
                            if(sql->SendSimpleQueryStr(" "+sql->crypt->mdEncrypt("students",sql->students_crypt_key)+
                                                       " SET "+updFields+" WHERE "+sql->crypt->mdEncrypt("id",sql->students_crypt_key)+
                                                       "="+curItem->text(1)+" AND "+
                                                       sql->crypt->mdEncrypt("group_id",sql->students_crypt_key)+"="+
                                                       curItem->parent()->text(1)+";")){
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
bool admin_form::sendStudData_toDB(QList<st_student> *data)
{
    bool result = true;
    for(int i=0; i<data->count();i++){
        if(sql->uniqGroup(data->at(i).group.code,true)){
            sql->addGroup(data->at(i).group.code);
        }
        if(sql->uniqStudent(data->at(i).surname,
                            data->at(i).name,
                            data->at(i).patronymic,
                            sql->getGroupIdByCode(data->at(i).group.code).toString(),true)){
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
    QList<st_student> impData;

    QFile file(QFileDialog::getOpenFileName(this, tr("Open files"),"/home/", "(*.csv)"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList strings;
        QTextStream in(&file);
        const int step = 10;
        while (!in.atEnd()) {
            st_student new_stud;
            strings = in.readLine().split(";");

            new_stud.group.code = strings.at(0).trimmed();
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
//  !!!! --- tab email --- !!!! {{
void admin_form::clearEMailTable()
{
    for(int r=ui->tableWidget_email->rowCount(); r>0; r--){
        ui->tableWidget_email->removeRow(r-1);
    }
}
//
void admin_form::on_groupBox_SendEMail_clicked()
{
    sql->set_sendEMail(ui->groupBox_SendEMail->isChecked());
}
//
void admin_form::getEMailAddrList()
{
    this->setCursor(Qt::BusyCursor);
    clearEMailTable();
    ui->tableWidget_email->hideColumn(2);
    QList<st_recipient> addr_list = sql->getEMailAddreses();
    if(addr_list.count() > 0){
        ui->tableWidget_email->setRowCount(addr_list.count());
        for(int i = 0; i < addr_list.count(); i++){
            ui->tableWidget_email->setItem(i,0,new QTableWidgetItem(addr_list.at(i).name));
            ui->tableWidget_email->setItem(i,1,new QTableWidgetItem(addr_list.at(i).address));
            ui->tableWidget_email->setItem(i,2,new QTableWidgetItem(addr_list.at(i).id));
        }
    }
    ui->tableWidget_email->resizeColumnsToContents();
    editAddr->setEnabled(false);
    delAddr->setEnabled(false);
    this->setCursor(Qt::ArrowCursor);
}
//
void admin_form::on_tableWidget_email_clicked(const QModelIndex &index)
{
    editAddr->setEnabled(true);
    delAddr->setEnabled(true);
}
//
void admin_form::on_action_addAddr_triggered()
{
    email_dlg addr_Data;

    if(addr_Data.exec()){
        if(address_correct(addr_Data.getAddress())){
            st_recipient new_data;
            new_data.name = addr_Data.getRecipient();
            new_data.address = addr_Data.getAddress();
            sql->addEMailAddr(&new_data);
            getEMailAddrList();
        }
        else{
            QMessageBox::critical(this,tr("Error"), tr("Invalid e-mail address.")+"\n"+addr_Data.getAddress());
        }
    }
}
//
void admin_form::on_action_editAddr_triggered()
{
    email_dlg addr_Data;
    addr_Data.setRecipient(ui->tableWidget_email->item(ui->tableWidget_email->currentItem()->row(),0)->text().trimmed());
    addr_Data.setAddress(ui->tableWidget_email->item(ui->tableWidget_email->currentItem()->row(),1)->text().trimmed());
    if(addr_Data.exec()){
        if(address_correct(addr_Data.getAddress())){
            st_recipient new_data;
            new_data.name = addr_Data.getRecipient();
            new_data.address = addr_Data.getAddress();
            new_data.id = ui->tableWidget_email->item(ui->tableWidget_email->currentItem()->row(),2)->text().trimmed();
            if(sql->updEMailAddr(&new_data)){
                getEMailAddrList();
            }
        }
        else{
            QMessageBox::critical(this,tr("Error"), tr("Invalid e-mail address.")+"\n"+addr_Data.getAddress());
        }
    }
}
//
void admin_form::on_action_delAddr_trigered()
{
    st_recipient del_data;
    del_data.name=ui->tableWidget_email->item(ui->tableWidget_email->currentItem()->row(),0)->text().trimmed();
    del_data.address = ui->tableWidget_email->item(ui->tableWidget_email->currentItem()->row(),1)->text().trimmed();
    del_data.id = ui->tableWidget_email->item(ui->tableWidget_email->currentItem()->row(),2)->text().trimmed();
    int ret = QMessageBox::question(this, tr("Removing address"),
                                    tr("Are you shure want delete address")+" \""+del_data.address+"\" \n "+
                                    tr("for recipient")+" "+del_data.name+"?",
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if(ret == QMessageBox::Yes){
        if(sql->delEMailAddr(&del_data)){
            getEMailAddrList();
        }
    }
}
//
void admin_form::on_action_SMTP_settings_triggered()
{
    QList<st_recipient> addreses_from_db = sql->getEMailAddreses();
    smtp_set *smtp_settings = new smtp_set(&addreses_from_db);

    st_smtp db_data = sql->getSMTP();
    smtp_settings->setServer(db_data.server);
    smtp_settings->setPort(db_data.port);
    smtp_settings->setUseSSL(db_data.ssl);
    smtp_settings->setMailFrom(db_data.mail_from);
    smtp_settings->setLogin(db_data.username);
    smtp_settings->setPassword(db_data.password);

    if(smtp_settings->exec()){
        db_data.server = smtp_settings->getServer().trimmed();
        db_data.port = smtp_settings->getPort();
        db_data.ssl = smtp_settings->getUseSSL();
        db_data.mail_from = smtp_settings->getMailFrom();
        db_data.username = smtp_settings->getLogin().trimmed();
        db_data.password = smtp_settings->getPassword().trimmed();
        sql->updSMTP(&db_data);
    }
}

//
//  !!!! --- tab email --- !!!! }}
//  !!!! --- tab tests --- !!!! {{
void admin_form::on_pushButton_createTest_clicked()
{
    this->setCursor(Qt::BusyCursor);
    test_mod_dlg *dlg = new test_mod_dlg();
    loadQuestions(dlg->takeTW_DBQ(),0);

    if(dlg->exec()){
        qDebug() << "Yahoo";
    }
    delete dlg;
    this->setCursor(Qt::ArrowCursor);
}
//  !!!! --- tab tests --- !!!! }}
