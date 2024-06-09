#include "entrancewindow.h"
#include "./ui_entrancewindow.h"
#include "registerwindow.h"

#include <iostream>

EntranceWindow::EntranceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EntranceWindow)
    , m_AuthenticationModule()
    , m_DataManagementModule()
{
    ui->setupUi(this);
    m_DataManagementModule.InitializeDB("./FinanceManager.db");
    m_DataManagementModule.CreateLoginTable();
}

EntranceWindow::~EntranceWindow()
{
    delete ui;
}

void EntranceWindow::on_pushButtonRegister_clicked()
{
    this->hide();
    RegisterWindow m_registerWindow{&m_DataManagementModule};
    m_registerWindow.resize(this->size());
    m_registerWindow.setGeometry(this->geometry());
    m_registerWindow.setModal(true);
    m_registerWindow.exec();
    this->show();
}


void EntranceWindow::on_pushButtonOk_clicked()
{
    const std::string user_name = ui->lineEditUserName->text().toStdString();
    std::string passport{};
    m_DataManagementModule.SelectRowLoginTable(user_name, passport);
    if(passport.empty())
    {
        ui->labelWarning->setText("User does not exist.");
    }
    else
    {
        // TODO start implementing main window
    }

}

