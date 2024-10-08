#include "stackedwindows.h"
#include "ui_stackedwindows.h"
#include <ControlModule/ControlModule.h>
#include <UIModule/DonutBreakdownChart.h>

#include <exception>

#include <thread>// delete me
#include <chrono>

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
    bool return_val = 
        control_module_.pushButtonEPOk_clicked(ui->lineEditEPUserName->text().toStdString(), actual_passport);

    if(return_val && ui->lineEditEPPassword->text().toStdString() == actual_passport && std::string{} != actual_passport)
    {
        ui->stackedWidget->setCurrentIndex(static_cast<int>(pageNumbers::MainPage));
        ui->stackedWidget->resize(MainPageSize);
        this->resize(MainPageSize);
        // control_module->getUserInfo();
        

        auto series1 = new QPieSeries;
        series1->setName("Fossil fuels");
        series1->append("Oil", 353295);
        series1->append("Coal", 188500);
        series1->append("Natural gas", 148680);
        series1->append("Peat", 94545);

        auto series2 = new QPieSeries;
        series2->setName("Renewables");
        series2->append("Wood fuels", 319663);
        series2->append("Hydro power", 45875);
        series2->append("Wind power", 1060);
        //series2->append() // accepts SLICE
        // Pie SLICE
        // Pie SERIES
        // QChart

        auto series3 = new QPieSeries;
        series3->setName("Others");
        series3->append("Nuclear energy", 238789);
        series3->append("Import energy", 37802);
        series3->append("Other", 32441);
        
        auto donutBreakdown = new DonutBreakdownChart(nullptr, Qt::WindowFlags::fromInt(0) );
        donutBreakdown->setAnimationOptions(QChart::AllAnimations);
        donutBreakdown->setTitle("Total consumption of energy in Finland 2010");
        donutBreakdown->legend()->setAlignment(Qt::AlignRight);

        ui->graphicsView->setChart(donutBreakdown);

        donutBreakdown->addBreakdownSeries(series1, Qt::red);
        donutBreakdown->addBreakdownSeries(series2, Qt::darkGreen);
        donutBreakdown->addBreakdownSeries(series3, Qt::darkBlue);


        
    }
    else if(false == return_val)
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
