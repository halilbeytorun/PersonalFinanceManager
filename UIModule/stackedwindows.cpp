#include "stackedwindows.h"
#include "ui_stackedwindows.h"

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

StackedWindows::StackedWindows(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StackedWindows)
    , m_AuthenticationModule()
    , m_DataManagementModule()
{
    ui->setupUi(this);
    auto return_code = m_DataManagementModule.InitializeDB("/home/halil/workspace/PersonalFinanceManager/SQlitedatabase.db");
    if(return_code != 0)
        throw std::domain_error{"Database initialization error"};
    return_code = m_DataManagementModule.CreateLoginTable();
    if(return_code != 0)
        throw std::domain_error{"Database table creation error"};
   
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
        if(ui->lineEditRPPassword->text() == ui->lineEditRPPasswordAgain->text()
        && !ui->lineEditRPUserName->text().isEmpty() && !ui->lineEditRPPassword->text().isEmpty())
        {
            if (int return_code = m_DataManagementModule.InsertLoginTable(ui->lineEditRPUserName->text().toStdString(),
            ui->lineEditRPPassword->text().toStdString() );
            return_code == 0 )
            {
                ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::EntrancePage));
            }
            else
            {
                ui->labelRPWarning->setText("username is used");
            }
        }
        else if(ui->lineEditRPUserName->text().isEmpty())
        {
            ui->labelRPWarning->setText("Enter a user name");
        }
        else if(ui->lineEditRPPassword->text().isEmpty())
        {
            ui->labelRPWarning->setText("Enter a passport");
        }
        else
        {
            ui->labelRPWarning->setText("Passports does not match");
        }
}




void StackedWindows::on_pushButtonEPOk_clicked()
{
    std::string actual_passport{};
    m_DataManagementModule.SelectRowLoginTable(ui->lineEditEPUserName->text().toStdString(), actual_passport);
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

