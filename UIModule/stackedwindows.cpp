#include "stackedwindows.h"
#include "ui_stackedwindows.h"
#include <ControlModule/ControlModule.h>
#include <UIModule/DonutBreakdownChart.h>
#include <UIModule/DonutBreakdownMainSlice.h>

#include <AuthenticationModule/AuthenticationModule.h>  // TODO: DELETE ME WHEN DB PART IS IMPELEMNTED

#include <exception>

#include <chrono>

AuthenticationModule::UserInfo temporary_user_info{};   // TODO DELETE ME
const std::string unused_name{"Unused"};

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

    Qt::GlobalColor getNextColorForNewSeries()
    {
    //         enum GlobalColor {
    //     color0,
    //     color1,
    //     black,
    //     white,
    //     darkGray,
    //     gray,
    //     lightGray,
    //     red,
    //     green,
    //     blue,
    //     cyan,
    //     magenta,
    //     yellow,
    //     darkRed,
    //     darkGreen,
    //     darkBlue,
    //     darkCyan,
    //     darkMagenta,
    //     darkYellow,
    //     transparent
    // };
        static Qt::GlobalColor color_counter{Qt::lightGray};
        color_counter = (Qt::GlobalColor) (color_counter + 1);
        if(Qt::transparent == color_counter)
        {
            color_counter = Qt::darkGray;
        }
        return color_counter;
    }

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
        
        ui->labelMPGreeting->setText(QString{"Welcome "} + ui->lineEditEPUserName->text() + QString{"! Please enter your monthly revanue from scratch since our DB is being updated!"});

        // TODO: Database data getting step must be here.

        // TODO: create a function for that later.
        // TODO: category selection must be not touchable before having a monthly revanue
        DisableMP_SpendingCategory();

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

void StackedWindows::on_pushButtonMPSpendingCategoryOk_clicked()
{
    bool return_value{};
    const double category_value = ui->lineEditMPSpendingCategoryValue->text().toDouble(&return_value);
    const std::string category_name = ui->lineEditMPSpendingCategoryName->text().toStdString();
    
    if(!return_value)
    {
        ui->labelMPSpendingCategoryWarning->setText("Please enter valid number");
        return;
    }

    if(category_value > std::get<0>(temporary_user_info.spent_category[unused_name]))
    {
        ui->labelMPSpendingCategoryWarning->setText("The number is bigger than unused area!");
        return;
    }


   ui->labelMPSpendingCategoryWarning->setText("");

    // TODO create function for category addition purpose    
    temporary_user_info.spent_category[category_name] = std::tuple<double, double, std::map<std::string,double>>{category_value, category_value, {}};
    std::get<0>(temporary_user_info.spent_category[unused_name]) = std::get<0>(temporary_user_info.spent_category[unused_name]) - category_value;
    std::get<1>(temporary_user_info.spent_category[unused_name]) = std::get<0>(temporary_user_info.spent_category[unused_name]);


    // TODO: This must be better somehow
    auto donutBreakdown = static_cast<DonutBreakdownChart*>(ui->graphicsView->chart());
    static_cast<QPieSeries*>(donutBreakdown->series().first())->slices().first()->setValue(std::get<0>(temporary_user_info.spent_category[unused_name]));
    static_cast<DonutBreakdownMainSlice*>(static_cast<QPieSeries*>(donutBreakdown->series().first())->slices().first())->breakdownSeries()->slices().first()->setValue(std::get<0>(temporary_user_info.spent_category[unused_name]));
    donutBreakdown->recalculateAngles();
    donutBreakdown->updateLegendMarkers();
    

    auto series1 = new QPieSeries;
    series1->setName(QString::fromStdString(category_name));
    series1->append(QString::fromStdString(category_name), category_value);
    donutBreakdown->addBreakdownSeries(series1, getNextColorForNewSeries());
}


void StackedWindows::on_pushButtonMPMonthlyRevanueOk_clicked()
{
    bool return_value{};
    const double monthly_revanue = ui->lineEditMPMonthlyRevanue->text().toDouble(&return_value);
    if(!return_value)
    {
        ui->labelMPMonthlyRevanueWarning->setText("Please enter valid number");
        return;
    }
    ui->labelMPMonthlyRevanueWarning->setText("");

    // TODO create function for category addition purpose    
    temporary_user_info.monthlyIncome = monthly_revanue;
    temporary_user_info.spent_category[unused_name] = std::tuple<double, double, std::map<std::string,double>>{monthly_revanue, monthly_revanue, {}};



    auto donutBreakdown = new DonutBreakdownChart(nullptr, Qt::WindowFlags::fromInt(0) );
    donutBreakdown->setAnimationOptions(QChart::AllAnimations);
    donutBreakdown->legend()->setAlignment(Qt::AlignRight);
    ui->graphicsView->setChart(donutBreakdown);

    auto series_unused_money = new QPieSeries;
    series_unused_money->setName(QString::fromStdString(unused_name));
    series_unused_money->append(QString::fromStdString(unused_name), monthly_revanue);
    donutBreakdown->addBreakdownSeries(series_unused_money, getNextColorForNewSeries());

    ui->graphicsView->chart()->setTitle("Monthly Revanue spending is: " + QString::number(monthly_revanue));
    EnableMP_SpendingCategory();
}

void StackedWindows::DisableMP_SpendingCategory()
{
    ui->lineEditMPSpendingCategoryName->setEnabled(false);
    ui->lineEditMPSpendingCategoryValue->setEnabled(false);
    ui->labelMPSpendingCategory->setEnabled(false);
    ui->labelMPSpendingCategoryName->setEnabled(false);
    ui->labelMPSpendingCategoryValue->setEnabled(false);
    ui->labelMPSpendingCategoryWarning->setEnabled(false);
    ui->pushButtonMPSpendingCategoryOk->setEnabled(false);
}

void StackedWindows::EnableMP_SpendingCategory()
{
    ui->lineEditMPSpendingCategoryName->setEnabled(true);
    ui->lineEditMPSpendingCategoryValue->setEnabled(true);
    ui->labelMPSpendingCategory->setEnabled(true);
    ui->labelMPSpendingCategoryName->setEnabled(true);
    ui->labelMPSpendingCategoryValue->setEnabled(true);
    ui->labelMPSpendingCategoryWarning->setEnabled(true);    
    ui->pushButtonMPSpendingCategoryOk->setEnabled(true);
}

