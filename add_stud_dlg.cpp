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
    result.grp_id = ui->comboBox_Groups->currentData().toInt();
    result.grp_code = ui->comboBox_Groups->currentText();

    return result;
}
//
full_stud_data add_stud_dlg::get_full_stud_data()
{
    full_stud_data result;
    result.grp = get_group_data();
    result.stud = get_stud_data();

    return result;
}
//
QString add_stud_dlg::get_lineEdit_Name()
{
    return ui->lineEdit_Name->text().trimmed();
}

QString add_stud_dlg::get_lineEdit_Surename()
{
    return ui->lineEdit_Surename->text().trimmed();
}
//
QString add_stud_dlg::get_lineEdit_Patronymic()
{
    return ui->lineEdit_Patronymic->text().trimmed();
}
//
