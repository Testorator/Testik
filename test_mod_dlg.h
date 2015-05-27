#ifndef TEST_CREATE_DLG_H
#define TEST_CREATE_DLG_H

#include <QDialog>

namespace Ui {
class test_create_dlg;
}

class test_create_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit test_create_dlg(QWidget *parent = 0);
    ~test_create_dlg();

private:
    Ui::test_create_dlg *ui;
};

#endif // TEST_CREATE_DLG_H
