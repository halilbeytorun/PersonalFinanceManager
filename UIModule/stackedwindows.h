#ifndef STACKEDWINDOWS_H
#define STACKEDWINDOWS_H

#include <QWidget>

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
private:
    Ui::StackedWindows *ui;
    ControlModule& control_module_;
};

#endif // STACKEDWINDOWS_H
