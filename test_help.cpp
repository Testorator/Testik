#include "test_help.h"
#include "ui_test_help.h"

test_help::test_help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test_help)
{
    ui->setupUi(this);
}

test_help::~test_help()
{
    delete ui;
}
