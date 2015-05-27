#ifndef ADMIN_FORM_H
#define ADMIN_FORM_H

#include "change_admin_pw_dialog.h"
#include "dbfunc.h"
#include "stud_dlg.h"
#include "theme_dlg.h"
#include "question_mod_dialog.h"

#include <QApplication>
#include <QMainWindow>

namespace Ui {
class admin_form;
}

class admin_form : public QMainWindow
{
    Q_OBJECT
protected:
    sql_cl *sql;

public:
    explicit admin_form(QWidget *parent = 0);
    ~admin_form();

private slots:
    void on_actionChange_admin_password_triggered();
    // db
    void on_pushButton_AddDB_clicked();
    void on_listWidget_DB_clicked();
    // students
    void on_actionAddGroup_triggered(QString group_code = 0);
    void on_actionAddStud_triggered();
    void on_pushButton_Edit_Stud_clicked();
    void on_toolButton_Add_Stud_clicked();
    void on_treeWidget_students_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_Import_Stud_clicked();
    void on_pushButton_Delete_Stud_clicked();
    void on_action_clearGroup_clicked();
    void on_pushButton_DelDB_clicked();
    void on_treeWidget_students_itemClicked(QTreeWidgetItem *item, int column);
    // questions

    void on_toolButton_Add_Quest_clicked();
    void on_action_addQuest_triggered();
    void on_action_addTheme_triggered();
    void on_pushButton_Edit_Quest_clicked();
    void set_questions_buttons_availablity(QTreeWidgetItem *item);
    void on_pushButton_Del_Quest_clicked();

    // email
    void on_groupBox_SendEMail_clicked();
    void on_action_addAddr_triggered();
    void on_action_editAddr_triggered();
    void on_action_delAddr_trigered();
    void on_action_SMTP_settings_triggered();
    void on_tableWidget_email_clicked(const QModelIndex &index);

    void on_pushButton_createTest_clicked();

private:
    Ui::admin_form *ui;

    struct st_QTWI{
        QString parent_id;
        QTreeWidgetItem *qtwi;
    };

    QString DBPath = QApplication::applicationDirPath()+"/data/";
    QAction *act_addGroup, *act_addStud, *act_addTheme, *act_addQuest, *addAddr, *editAddr, *delAddr, *SMTP_settings;
    void getDataBases();
    void loadQuestions(QTreeWidget *curQTW, int q_type);
    // students
    void getStudentsList();
    void setAvailabilityOfItems(bool val);
    bool prepareAddStudDlg(stud_dlg *dlg);
    bool sendStudData_toDB(QList<st_student> *data);
    // questions
    QTreeWidget* get_curQTW(int q_type = -1);
    void getQuestionList(int q_type = -1);
    void prepareThemesDlg(theme_dlg *dlg, QTreeWidget *curQTW, QString exclude_id = 0);
    void prepareQuestDlg(question_mod_dialog *dlg);
    // email
    void clearEMailTable();
    void getEMailAddrList();

};

#endif // ADMIN_FORM_H
