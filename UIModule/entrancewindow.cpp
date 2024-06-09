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

