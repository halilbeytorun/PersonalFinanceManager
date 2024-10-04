#include "stackedwindows.h"
#include "ui_stackedwindows.h"
#include <ControlModule/ControlModule.h>

#include <exception>

enum class pageNumbers : int
{
    EntrancePage,
    RegisterPage,
    MainPage
};

namespace
{
    QSize RegisterPageSize = QSize{611, 301};
    QSize MainPageSize = QSize{1103, 633};
}

StackedWindows::StackedWindows(ControlModule& control_module, QWidget *parent)
    : control_module_{control_module}
    , QWidget{parent}
    , ui{new Ui::StackedWindows}
{
    ui->setupUi(this);
   
   ui->stackedWidget->resize(RegisterPageSize);
   this->resize(RegisterPageSize);
   ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::EntrancePage));

    QPalette pal = QPalette();

    // set black background
    // Qt::black / "#000000" / "black"
    pal.setColor(QPalette::Window, Qt::cyan);

    this->setAutoFillBackground(true); 
    this->setPalette(pal);
}

StackedWindows::~StackedWindows()
{
    delete ui;
}

void StackedWindows::on_pushButtonEPRegister_clicked()
{
    ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::RegisterPage));
}


void StackedWindows::on_pushButtonRPRegister_clicked()
{

        if(ui->lineEditRPUserName->text().isEmpty())
        {
            ui->labelRPWarning->setText("Enter a user name");
        }
        else if(ui->lineEditRPPassword->text().isEmpty())
        {
            ui->labelRPWarning->setText("Enter a passport");
        }
        else if(ui->lineEditRPPassword->text() == ui->lineEditRPPasswordAgain->text())
        {
            if ( control_module_.pushButtonRPRegister_clicked(ui->lineEditRPUserName->text().toStdString(), ui->lineEditRPPassword->text().toStdString()))
            {
                ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::EntrancePage));
            }
            else
            {
                ui->labelRPWarning->setText("username is used");
            }
        }        
        else
        {
            ui->labelRPWarning->setText("Passports does not match");
        }
}




void StackedWindows::on_pushButtonEPOk_clicked()
{
    std::string actual_passport{};
    control_module_.pushButtonEPOk_clicked(ui->lineEditEPUserName->text().toStdString(), actual_passport);
    
    if(ui->lineEditEPPassword->text().toStdString() == actual_passport && std::string{} != actual_passport)
    {
        ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::MainPage));
        ui->stackedWidget->resize(MainPageSize);
        this->resize(MainPageSize);
    }
    else if(std::string{} == actual_passport)
    {
        ui->labelEPWarning->setText("Wrong user name");
    }
    else
    {
        ui->labelEPWarning->setText("Wrong passport!");
    }
}


void StackedWindows::on_pushButtonRPCancel_clicked()
{
    ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::EntrancePage));
}

