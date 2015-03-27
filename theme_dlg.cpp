#include "theme_dlg.h"
#include "ui_theme_dlg.h"

theme_dlg::theme_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::theme_dlg)
{
    ui->setupUi(this);
    ui->treeWidget_Themes->hideColumn(1);
    ui->treeWidget_Themes->setSortingEnabled(true);
    ui->treeWidget_Themes->sortByColumn(0,Qt::AscendingOrder);
}
//
theme_dlg::~theme_dlg()
{
    delete ui;
}
//
void theme_dlg::clear_PThemes()
{
    ui->treeWidget_Themes->clear();
}
//
QTreeWidgetItem* theme_dlg::getQTWItemByID(QString id)
{
    QTreeWidgetItem * result;
    QList<QTreeWidgetItem * > Items = ui->treeWidget_Themes->findItems(id,Qt::MatchExactly,1);
    if(Items.count() > 0){
        result = Items.at(0);
    }
    return result;
}
//
void theme_dlg::add_PTheme(QString item, QString itemID, QString parent_ID)
{
    QTreeWidgetItem * parentItem_QTWI;
    QStringList newItem;
    newItem << item << itemID;
    bool hasParent = false;
    if(!parent_ID.isNull() && !parent_ID.isEmpty() && parent_ID.trimmed().length() > 0){
        parentItem_QTWI = getQTWItemByID(parent_ID);
        if(parentItem_QTWI) hasParent = true;
    }

    if(hasParent){
        parentItem_QTWI->addChild(new QTreeWidgetItem(newItem));
    }
    else{
        ui->treeWidget_Themes->insertTopLevelItem(0,new QTreeWidgetItem(newItem));
    }
}
//
void theme_dlg::set_current_PTheme(QString id)
{
        QTreeWidgetItem * parentItem_QTWI = getQTWItemByID(id);
        if(parentItem_QTWI) ui->treeWidget_Themes->setCurrentItem(parentItem_QTWI);
}
//
QString theme_dlg::get_ThemeName()
{
   return  ui->lineEdit_ThemeName->text().trimmed();
}
//
QString theme_dlg::get_PThemeID()
{
    QString result = 0;
    QTreeWidgetItem *curItem = ui->treeWidget_Themes->currentItem();
    if(curItem) result = curItem->text(1).trimmed();

    return result;
}
//

