#include "theme_dlg.h"
#include "ui_theme_dlg.h"

theme_dlg::theme_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::theme_dlg)
{
    ui->setupUi(this);
}
//
theme_dlg::~theme_dlg()
{
    delete ui;
}
//
void theme_dlg::clear_PThemes()
{
    ui->comboBox_PThemes->clear();
}
//
void theme_dlg::add_PTheme(QString item, QVariant itemID)
{
    ui->comboBox_PThemes->addItem(item,itemID);
}
//
void theme_dlg::set_current_PTheme(QString PThemeName)
{
    ui->comboBox_PThemes->setCurrentText(PThemeName);
}
//
QString theme_dlg::get_ThemeName()
{
    ui->lineEdit_ThemeName->text().trimmed();
}
//
QString theme_dlg::get_PThemeID()
{
    ui->comboBox_PThemes->currentData().toString();
}
//

