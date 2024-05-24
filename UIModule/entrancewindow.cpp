#include "entrancewindow.h"
#include "./ui_entrancewindow.h"

#include <iostream>

EntranceWindow::EntranceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EntranceWindow)
    , m_AuthenticationModule()
    , m_DataManagementModule()
{
    ui->setupUi(this);
}

EntranceWindow::~EntranceWindow()
{
    delete ui;
}
