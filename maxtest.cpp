#include "maxtest.h"
#include "ui_maxtest.h"

#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMessageBox>

Maxtest::Maxtest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Maxtest)
{
    ui->setupUi(this);
}

Maxtest::~Maxtest()
{
    delete ui;
}

void Maxtest::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
   // db.setHostName("localhost");
    //db.setDatabaseName("D:/Maks/project/Testik/data/BLANK.QLT");
    db.setDatabaseName("/Users/maksimserbakov/Documents/work/Testik/Maxtest/data/BLANK.QLT");



  db.setUserName("USER");
  db.setPassword("user");
    db.open();
    qDebug() << "db is open: " << db.isOpen();
    qDebug() << "db last error: " << db.lastError().text();

}
