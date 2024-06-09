#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>

namespace Ui {
class RegisterWindow;
}

class DataManagementModule;

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(DataManagementModule* _m_DataManagementModule, QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_pushButtonRegister_clicked();

private:
    Ui::RegisterWindow *ui;
    DataManagementModule* m_DataManagementModule;
};

#endif // REGISTERWINDOW_H
