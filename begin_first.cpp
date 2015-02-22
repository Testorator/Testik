#include "begin_first.h"
#include "ui_begin_first.h"

Begin_first::Begin_first(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Begin_first)
{
    ui->setupUi(this);
}

Begin_first::~Begin_first()
{
    delete ui;
}
