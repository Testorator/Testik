#include "admin_form.h"
#include "ui_admin_form.h"
#include "change_admin_pw_dialog.h"
#include "question_mod_dialog.h"

#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

admin_form::admin_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin_form)
{
    ui->setupUi(this);
    ui->listWidget_DB->clear();

    getDataBases();
}

admin_form::~admin_form()
{
    delete ui;
}
//
void admin_form::on_actionChange_admin_password_triggered()
{
    change_admin_pw_dialog chPW_dialog;
    chPW_dialog.exec();
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
else
{
        QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
        db.setDatabaseName("BLANK.QLT");
        db.setUserName("USER");
        db.setPassword("user");
        db.open();
        if(!db.open()){
          qDebug() << db.lastError().text();
        }
        else {

           qDebug() << "success";
        }
    }
}

