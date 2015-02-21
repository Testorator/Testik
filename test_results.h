#ifndef TEST_RESULTS_H
#define TEST_RESULTS_H

#include <QWidget>

namespace Ui {
class test_results;
}

class test_results : public QWidget
{
    Q_OBJECT
    Q_ENUMS(resultType)

public:
    explicit test_results(resultType resType, QWidget *parent = 0);
    ~test_results();
    enum resultType { Learn, Test };

private slots:
    void on_pushButton_SaveOrReturn_clicked();

    void on_pushButton_MailOrRetry_clicked();

    void on_pushButton_RetryOrStartTest_clicked();

private:
    Ui::test_results *ui;
    resultType curResType;
};

#endif // TEST_RESULTS_H
