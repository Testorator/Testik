#ifndef MAXTEST_H
#define MAXTEST_H

#include <QMainWindow>

namespace Ui {
class Maxtest;
}

class Maxtest : public QMainWindow
{
    Q_OBJECT

public:
    explicit Maxtest(QWidget *parent = 0);
    ~Maxtest();

private slots:

private:
    Ui::Maxtest *ui;
};

#endif // MAXTEST_H
