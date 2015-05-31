#include "test_mod_dlg.h"
#include "ui_test_mod_dlg.h"
#include <QMessageBox>

test_mod_dlg::test_mod_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::test_mod_dlg)
{
    ui->setupUi(this);
}
//
test_mod_dlg::~test_mod_dlg()
{
    delete ui;
}
//
QTreeWidget* test_mod_dlg::takeTW_DBQ()
{
    return ui->treeWidget_DBQuestions;
}
//
QTreeWidget* test_mod_dlg::takeTW_TQ()
{
    return ui->treeWidget_testQuestions;
}
//
void test_mod_dlg::on_buttonBox_accepted()
{
    this->accept();
}
//
void test_mod_dlg::on_buttonBox_rejected()
{
    this->reject();
}
//
void test_mod_dlg::on_toolButton_addOne_clicked()
{
    QTreeWidgetItem *curTWI = ui->treeWidget_DBQuestions->currentItem();
    if(curTWI){
        int x= 0;
    }
    else{
        QMessageBox::information(this,tr("Information"),tr("Plrqse select question or theme, before adding"));
    }
}
