#include "admin_form.h"
#include "ui_admin_form.h"
#include "change_admin_pw_dialog.h"
#include "question_mod_dialog.h"

#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QtSql/QSqlError>

admin_form::admin_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin_form)
{
    ui->setupUi(this);
    this->setWindowTitle(QApplication::applicationName()+": admin mode");
    getDataBases();
    db = QSqlDatabase::addDatabase("QIBASE");
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
        qDebug() << "success";
    }
    else{
        QMessageBox::critical(this,
                              tr("Error"),
                              "database: "+QApplication::applicationDirPath()+"/data/"+db_file+"\n"+db.lastError().text());
    }
}
//
void admin_form::createDBStruct(){

//    CREATE TABLE EMAIL_ADDRESES (RECIPIENT_NAME VARCHAR(50),ADDRESS VARCHAR(50) NOT NULL)

}
