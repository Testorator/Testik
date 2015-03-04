#ifndef BEGIN_FIRST_H
#define BEGIN_FIRST_H

#include <QWidget>

namespace Ui {
class Begin_first;
}

class Begin_first : public QWidget
{
    Q_OBJECT

public:
    explicit Begin_first(QWidget *parent = 0);
    ~Begin_first();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Begin_first *ui;
};

#endif // BEGIN_FIRST_H
