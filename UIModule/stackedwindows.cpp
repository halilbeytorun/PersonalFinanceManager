#include "stackedwindows.h"
#include "ui_stackedwindows.h"

StackedWindows::StackedWindows(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StackedWindows)
    , m_AuthenticationModule()
    , m_DataManagementModule()
{
    ui->setupUi(this);
}

StackedWindows::~StackedWindows()
{
    delete ui;
}

void StackedWindows::on_pushButtonEPRegister_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
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
                ui->stackedWidget->setCurrentIndex(0);
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


// TODO
// void MainWindow::on_pushButtonOk_clicked()
// {
//     // const std::string user_name = ui->lineEditUserName->text().toStdString();
//     // std::string passport{};
//     // m_DataManagementModule.SelectRowLoginTable(user_name, passport);
//     // if(passport.empty())
//     // {
//     //     ui->labelWarning->setText("User does not exist.");
//     // }
//     // else
//     // {
//     //     MainWindow mainWindow{};
//     //     mainWindow.resize(this->size());
//     //     mainWindow.setGeometry(this->geometry());
//     //     mainWindow.setModal(true);
//     //     mainWindow.show();
//     //     this->hide();
//     //     mainWindow.exec();
//     //     this->show();
//     // }

// }



