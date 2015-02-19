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

    db.setDatabaseName("d:\test.qfb");
 //   db.setUserName("SYSDBA");
 //   db.setPassword("masterkey");
    db.open();
    QMessageBox::critical(this,"ERROR",db.lastError().text());
}
