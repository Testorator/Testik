#ifndef THEME_DLG_H
#define THEME_DLG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class theme_dlg;
}

class theme_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit theme_dlg(QWidget *parent = 0);
    ~theme_dlg();
    void clear_PThemes();
    void add_PTheme(QString item, QString itemID, QString parent_ID = 0);
    void set_current_PTheme(QString id);
    void set_current_ThemeName(QString thname);
    QString get_PThemeID();
    QString get_ThemeName();

private slots:

private:
    Ui::theme_dlg *ui;
    QTreeWidgetItem* getQTWItemByID(QString id);
};

#endif // THEME_DLG_H
