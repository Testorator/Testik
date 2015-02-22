#include "test_results.h"
#include "ui_test_results.h"
#include <QMessageBox>


test_results::test_results(resultType resType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test_results)
{
    ui->setupUi(this);

    curResType = resType;
    if(curResType == this->Learn){
        this->setWindowTitle(tr("Learning outcomes"));
        ui->label_result_type->setText('<html><head/><body><p align="center">'+tr("Learning outcomes")+'</p></body></html>');
        ui->pushButton_SaveOrReturn->setText(tr("Return"));
        ui->pushButton_MailOrRetry->setText(tr("Go re-test"));
        ui->pushButton_RetryOrStartTest->setText(tr("Start test"));
    }
    else {
        this->setWindowTitle(tr("The test result"));
        ui->label_result_type->setText('<html><head/><body><p align="center">'+tr("The test result")+'</p></body></html>');
        ui->pushButton_SaveOrReturn->setText(tr("Save results"));
        ui->pushButton_MailOrRetry->setText(tr("Send results by e-mail"));
        ui->pushButton_RetryOrStartTest->setText(tr("Go re-test"));
    }
}
//
test_results::~test_results()
{
    delete ui;
}
//
void test_results::on_pushButton_SaveOrReturn_clicked()
{
     if(curResType == this->Learn){
         QMessageBox::information(this,"INFO","The return function is under construction");
     }
     else{
         QMessageBox::information(this,"INFO","The function of saving results is under construction");
     }
}
//
void test_results::on_pushButton_MailOrRetry_clicked()
{
    if(curResType == this->Learn){
        QMessageBox::information(this,"INFO","The function of re-test is under construction");
    }
    else{
        QMessageBox::information(this,"INFO","The function of sending results by e-mail, is under construction");
    }
}
//
void test_results::on_pushButton_RetryOrStartTest_clicked()
{
    if(curResType == this->Learn){
        QMessageBox::information(this,"INFO","The function of start testing, is under construction");
    }
    else{
        QMessageBox::information(this,"INFO","The function of re-test is under construction");
    }
}
