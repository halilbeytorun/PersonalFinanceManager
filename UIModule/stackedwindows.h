#ifndef STACKEDWINDOWS_H
#define STACKEDWINDOWS_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>

class ControlModule;

namespace Ui {
class StackedWindows;
}

class StackedWindows : public QWidget
{
    Q_OBJECT

public:
    explicit StackedWindows(ControlModule& control_module, QWidget *parent = nullptr);
    ~StackedWindows();
private slots:
    void on_pushButtonEPRegister_clicked();
    void on_pushButtonRPRegister_clicked();
    void on_pushButtonEPOk_clicked();
    void on_pushButtonRPCancel_clicked();
    void on_pushButtonMPSpendingCategoryOk_clicked();

    void on_pushButtonMPMonthlyRevanueOk_clicked();

    void on_pushButtonMPRemoveSpendingCategoryOk_clicked();

private:
    void DisableMP_SpendingCategory();
    void EnableMP_SpendingCategory();
    Ui::StackedWindows *ui;
    ControlModule& control_module_;
};

#endif // STACKEDWINDOWS_H
