#ifndef ADMIN_FORM_H
#define ADMIN_FORM_H

#include "change_admin_pw_dialog.h"
#include "add_stud_dlg.h"
#include "dbfunc.h"

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
    void on_pushButton_clicked();
    void on_pushButton_AddDB_clicked();
    void on_listWidget_DB_clicked();
    void on_actionAddGroup_triggered(QString group_code = 0);
    void on_actionAddStud_triggered();
    void on_pushButton_Edit_Stud_clicked();
    void on_toolButton_Add_Stud_clicked();
    void on_treeWidget_students_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_Import_Stud_clicked();
    void on_pushButton_Delete_Stud_clicked();
    void on_action_clearGroup_clicked();

private:
    Ui::admin_form *ui;
    QSqlDatabase db;
    QAction *act_addGroup, *act_addStud;
    void getDataBases();
    void setAvailabilityOfItems(bool val);
    void getStudentsList();
    bool prepareAddStudDlg(add_stud_dlg *dlg);
    bool sendStudData_toDB(QList<st_stud> *data);

};

#endif // ADMIN_FORM_H
