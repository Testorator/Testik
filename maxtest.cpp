#include "maxtest.h"
#include "ui_maxtest.h"

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
