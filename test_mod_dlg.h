#ifndef TEST_MOD_DLG_H
#define TEST_MOD_DLG_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class test_mod_dlg;
}

class test_mod_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit test_mod_dlg(QWidget *parent = 0);
    ~test_mod_dlg();
    QTreeWidget* takeTW_DBQ();
    QTreeWidget* takeTW_TQ();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::test_mod_dlg *ui;
};

#endif // TEST_MOD_DLG_H
