#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "DataManagementModule/DataManagementModule.h"

RegisterWindow::RegisterWindow(DataManagementModule* _m_DataManagementModule, QWidget *parent)
    : QDialog(parent)
    , m_DataManagementModule(_m_DataManagementModule),ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_pushButtonRegister_clicked()
{
    if(ui->lineEditPassword->text() == ui->lineEditPasswordAgain->text() 
    && !ui->lineEditUserName->text().isEmpty() && !ui->lineEditPassword->text().isEmpty())
    {
        if (int return_code = m_DataManagementModule->InsertLoginTable(ui->lineEditUserName->text().toStdString(),
        ui->lineEditPassword->text().toStdString() );
        return_code == 0 )
        {
            this->done(0);
        }
        else
        {
            ui->labelWarning->setText("username is used");
        }
    }
    else if(ui->lineEditUserName->text().isEmpty())
    {
        ui->labelWarning->setText("Enter a user name");
    }
    else if(ui->lineEditPassword->text().isEmpty())
    {
        ui->labelWarning->setText("Enter a passport");
    }
    else
    {
        ui->labelWarning->setText("Passports does not match");
    }
}

