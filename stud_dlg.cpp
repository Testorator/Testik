#include "stud_dlg.h"
#include "ui_stud_dlg.h"

stud_dlg::stud_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stud_dlg)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/stud/stud"));
}

stud_dlg::~stud_dlg()
{
    delete ui;
}
//
void stud_dlg::comboBox_groups_clear()
{
    ui->comboBox_Groups->clear();
}
//
void stud_dlg::comboBox_groups_addItem(QString item, QVariant itemID)
{
    ui->comboBox_Groups->addItem(item,itemID);
}
//
void stud_dlg::comboBox_groups_set_curItem(QString item)
{
    ui->comboBox_Groups->setCurrentText(item);
}
//
QString stud_dlg::get_lineEdit_Name()
{
    return ui->lineEdit_Name->text().trimmed();
}
//
void stud_dlg::lineEdit_Name_setText(QString value)
{
    ui->lineEdit_Name->setText(value);
}
//
QString stud_dlg::get_lineEdit_Surname()
{
    return ui->lineEdit_Surname->text().trimmed();
}
//
void stud_dlg::lineEdit_Surname_setText(QString value)
{
    ui->lineEdit_Surname->setText(value);
}
//
QString stud_dlg::get_lineEdit_Patronymic()
{
    return ui->lineEdit_Patronymic->text().trimmed();
}
//
void stud_dlg::lineEdit_Patronymic_setText(QString value)
{
    ui->lineEdit_Patronymic->setText(value);
}
//
QVariant stud_dlg::get_group_id()
{
    return ui->comboBox_Groups->currentData();
}
//
QString stud_dlg::get_group_code()
{
    return ui->comboBox_Groups->currentText();
}
//
