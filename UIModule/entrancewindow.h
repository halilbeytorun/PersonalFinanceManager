#ifndef ENTRANCEWINDOW_H
#define ENTRANCEWINDOW_H

#include <QMainWindow>
#include "AuthenticationModule/AuthenticationModule.h"
#include "DataManagementModule/DataManagementModule.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class EntranceWindow;
}
QT_END_NAMESPACE

class EntranceWindow : public QMainWindow
{
    Q_OBJECT

public:
    EntranceWindow(QWidget *parent = nullptr);
    ~EntranceWindow();

private slots:

    void on_pushButtonRegister_clicked();

    void on_pushButtonOk_clicked();

private:
    Ui::EntranceWindow *ui;
    AuthenticationModule m_AuthenticationModule;
    DataManagementModule m_DataManagementModule;

};
#endif // ENTRANCEWINDOW_H
