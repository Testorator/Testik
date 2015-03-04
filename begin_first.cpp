#include "begin_first.h"
#include "ui_begin_first.h"

Begin_first::Begin_first(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Begin_first)
{
    ui->setupUi(this);
    ui->wg->hide();
}

Begin_first::~Begin_first()
{
    delete ui;
}

void Begin_first::on_pushButton_clicked()
{
    ui->wg->show();
    ui->widg->hide();
}

void Begin_first::on_pushButton_7_clicked()
{
    ui->wg->close();
    ui->widg->show();

}
