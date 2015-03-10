#include "add_stud_dlg.h"
#include "ui_add_stud_dlg.h"

add_stud_dlg::add_stud_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_stud_dlg)
{
    ui->setupUi(this);
}

add_stud_dlg::~add_stud_dlg()
{
    delete ui;
}
//
void add_stud_dlg::comboBox_groups_clear()
{
    ui->comboBox_Groups->clear();
}
//
void add_stud_dlg::comboBox_groups_addItem(QString item, QVariant itemID)
{
    ui->comboBox_Groups->addItem(item,itemID);
}
//
void add_stud_dlg::comboBox_groups_set_curItem(QString item)
{
    ui->comboBox_Groups->setCurrentText(item);
}
//
stud_data add_stud_dlg::get_stud_data()
{
    stud_data result;
    result.stud_name = get_lineEdit_Name();
    result.stud_surename = get_lineEdit_Surename();
    result.stud_patronymic = get_lineEdit_Patronymic();

    return result;
}
//
grp_data add_stud_dlg::get_group_data(){
    grp_data result;
    result.grp_id = get_group_id().toInt();
    result.grp_code = get_group_code();

    return result;
}
//
QString add_stud_dlg::get_lineEdit_Name()
{
    return ui->lineEdit_Name->text().trimmed();
}
//
void add_stud_dlg::lineEdit_Name_setText(QString value)
{
    ui->lineEdit_Name->setText(value);
}
//
QString add_stud_dlg::get_lineEdit_Surename()
{
    return ui->lineEdit_Surename->text().trimmed();
}
//
void add_stud_dlg::lineEdit_Surename_setText(QString value)
{
    ui->lineEdit_Surename->setText(value);
}
//
QString add_stud_dlg::get_lineEdit_Patronymic()
{
    return ui->lineEdit_Patronymic->text().trimmed();
}
//
void add_stud_dlg::lineEdit_Patronymic_setText(QString value)
{
    ui->lineEdit_Patronymic->setText(value);
}

//
QVariant add_stud_dlg::get_group_id()
{
    return ui->comboBox_Groups->currentData();
}
//
QString add_stud_dlg::get_group_code()
{
    return ui->comboBox_Groups->currentText();
}
//
