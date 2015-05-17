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

