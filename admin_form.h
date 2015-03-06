#ifndef ADMIN_FORM_H
#define ADMIN_FORM_H

#include "change_admin_pw_dialog.h"

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
    void on_actionAddGroup_triggered();
    void on_actionAddStud_triggered();

private:
    Ui::admin_form *ui;
    QSqlDatabase db;
    QAction *addGroup, *addStud;
    void getDataBases();
    void setAvailabilityOfItems(bool val);
    void getStudentsList();


};

#endif // ADMIN_FORM_H
