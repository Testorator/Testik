#ifndef TEST_HELP_H
#define TEST_HELP_H

#include <QWidget>

namespace Ui {
class test_help;
}

class test_help : public QWidget
{
    Q_OBJECT

public:
    explicit test_help(QWidget *parent = 0);
    ~test_help();

private:
    Ui::test_help *ui;
};

#endif // TEST_HELP_H
