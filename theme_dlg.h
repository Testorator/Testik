#ifndef THEME_DLG_H
#define THEME_DLG_H

#include <QDialog>

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
    void add_PTheme(QString item, QVariant itemID);
    void set_current_PTheme(QString PThemeName);
    QString get_PThemeID();
    QString get_ThemeName();

private:
    Ui::theme_dlg *ui;
};

#endif // THEME_DLG_H
