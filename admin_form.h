#ifndef ADMIN_FORM_H
#define ADMIN_FORM_H

#include "change_admin_pw_dialog.h"
#include "stud_dlg.h"
#include "theme_dlg.h"
#include "dbfunc.h"

#include <QApplication>
#include <QMainWindow>
#include <QtSql/QSqlDatabase>



namespace Ui {
class admin_form;
}



class admin_form : public QMainWindow
{
    Q_OBJECT

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
    // questions
    void on_toolButton_Add_Quest_clicked();
    void on_action_addTheme_triggered();

private:
    Ui::admin_form *ui;
    QString DBPath = QApplication::applicationDirPath()+"/data/";
    QSqlDatabase db;
    QAction *act_addGroup, *act_addStud, *act_addTheme, *act_addQuest;
    void getDataBases();
    // students
    void getStudentsList();
    void setAvailabilityOfItems(bool val);
    bool prepareAddStudDlg(stud_dlg *dlg);
    bool sendStudData_toDB(QList<st_stud> *data);
    // questions
    QTreeWidgetItem* findQParentByID(QTreeWidget *curQTW, int parent_id);
    void getQuestionList(int question_Type); // 0 - test; 1 - learn
    bool prepareThemesDlg(theme_dlg *dlg);

};

#endif // ADMIN_FORM_H
