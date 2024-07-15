#ifndef STACKEDWINDOWS_H
#define STACKEDWINDOWS_H

#include <QWidget>
#include "AuthenticationModule/AuthenticationModule.h"
#include "DataManagementModule/DataManagementModule.h"

namespace Ui {
class StackedWindows;
}

class StackedWindows : public QWidget
{
    Q_OBJECT

public:
    explicit StackedWindows(QWidget *parent = nullptr);
    ~StackedWindows();

private slots:
    void on_pushButtonEPRegister_clicked();

    void on_pushButtonRPRegister_clicked();

    void on_pushButtonEPOk_clicked();

private:
    Ui::StackedWindows *ui;
    AuthenticationModule m_AuthenticationModule;
    DataManagementModule m_DataManagementModule;
};

#endif // STACKEDWINDOWS_H
