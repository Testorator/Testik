#include "login_dlg.h"
#include "ui_login_dlg.h"
#include <QDir>
#include <QMessageBox>
#include <QDebug>

login_dlg::login_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_dlg)
{
    ui->setupUi(this);
    this->setWindowTitle(QApplication::applicationName()+": login");
    sql = new sql_cl();
    getDataBases();
    ui->comboBox_Groups->setEnabled(sql->dbIsOpen());
    ui->comboBox_Students->setEnabled(sql->dbIsOpen());
    ui->pushButton_Login->setEnabled(sql->dbIsOpen());
}
//
login_dlg::~login_dlg()
{
    delete sql;
    delete ui;
}
//
void login_dlg::getDataBases()
{
    this->setCursor(Qt::BusyCursor);
    QString DBPath = QApplication::applicationDirPath()+"/data/";
    QDir db_dir(DBPath,
                "*.qlt",
                QDir::Name,
                QDir::Files|QDir::Readable|QDir::Writable|QDir::NoSymLinks);

    QStringList db_files = db_dir.entryList();
    ui->comboBox_DB->clear();
    ui->comboBox_DB->addItem("...","");
    for(int i=0; i < db_files.count(); i++){
        QString curFile = db_files.at(i);
        QFileInfo f_info(DBPath+curFile);

        if(f_info.isReadable() && f_info.isWritable()){
            ui->comboBox_DB->addItem(QIcon(":db/database"),curFile.replace(".QLT","",Qt::CaseInsensitive),curFile);
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
void login_dlg::on_pushButton_Exit_clicked()
{
    QApplication::exit(0);
}
//
void login_dlg::on_comboBox_DB_currentIndexChanged(int index)
{
    QString db_file = ui->comboBox_DB->itemData(index).toString();
    if(!db_file.isEmpty()){
        bool db_is_open = sql->openDB(QApplication::applicationDirPath()+"/data/"+db_file+".QLT");
        ui->comboBox_Groups->setEnabled(db_is_open);
        if(db_is_open){
            ui->comboBox_Groups->clear();
            ui->comboBox_Groups->addItem("...","");
            QList<st_group> grp_list = sql->getGroups();
            for(int i = 0; i < grp_list.count(); i++){
                ui->comboBox_Groups->addItem(QIcon(":/stud/group"),grp_list.at(i).code,grp_list.at(i).id);
            }
        }
    }
}
//
void login_dlg::on_comboBox_Groups_currentIndexChanged(int index)
{
    QString selected_group = ui->comboBox_Groups->itemData(index).toString();
    if(!selected_group.isEmpty()){
        ui->comboBox_Students->setEnabled(true);
        ui->comboBox_Students->clear();
        ui->comboBox_Students->addItem("...","");
        QList<st_student> stud_list = sql->getStudents(selected_group);
        for(int i = 0; i < stud_list.count(); i++){
            ui->comboBox_Students->addItem(QIcon(":/stud/stud"),stud_list.at(i).surname+" "+stud_list.at(i).name+" "+stud_list.at(i).patronymic,
                                           stud_list.at(i).id);
        }
    }
    else{
        ui->comboBox_Students->clear();
        ui->comboBox_Students->setEnabled(false);
    }
}
//
void login_dlg::on_comboBox_Students_currentIndexChanged(int index)
{
       QString selected_stud = ui->comboBox_Students->itemData(index).toString();
       ui->pushButton_Login->setEnabled(!selected_stud.isEmpty());
}
