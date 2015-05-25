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
//    sql = new sql_cl();
//    getDataBases();
//    ui->comboBox_Groups->setEnabled(sql->dbIsOpen());
//    ui->comboBox_Students->setEnabled(sql->dbIsOpen());
//    ui->pushButton_Login->setEnabled(sql->dbIsOpen());
}
//
login_dlg::~login_dlg()
{
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

    for(int i=0; i < db_files.count(); i++){
        QString curFile = db_files.at(i);
        QFileInfo f_info(DBPath+curFile);

        if(f_info.isReadable() && f_info.isWritable()){
            ui->comboBox_DB->addItem(curFile.replace(".QLT","",Qt::CaseInsensitive),curFile);
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
    bool db_is_open = sql->openDB(QApplication::applicationDirPath()+"/data/"+db_file+".QLT");
    ui->comboBox_Groups->setEnabled(db_is_open);
    if(db_is_open){
        ui->comboBox_Groups->clear();
//        sql->getGroups();
    }
}
