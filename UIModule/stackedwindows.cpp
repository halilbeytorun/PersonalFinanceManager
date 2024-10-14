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
        static Qt::GlobalColor color_counter{Qt::yellow};
        color_counter = (Qt::GlobalColor) (color_counter + 1);
        if(Qt::transparent == color_counter)
        {
            color_counter = Qt::yellow;
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
    if(category_value <= 0)
    {
        ui->labelMPSpendingCategoryWarning->setText("Please enter valid number");
        return;        
    }
    if(category_value > std::get<0>(temporary_user_info.spent_category[unused_name]))
    {
        ui->labelMPSpendingCategoryWarning->setText("The number is bigger than unused area!");
        return;
    }
    if(temporary_user_info.spent_category.find(category_name) != temporary_user_info.spent_category.end())
    {
        ui->labelMPSpendingCategoryWarning->setText("The category already exists!");
        return;
    }


   ui->labelMPSpendingCategoryWarning->setText("");

    // TODO create function for category addition purpose    
    temporary_user_info.spent_category[category_name] = std::tuple<double, double, std::map<std::string,double>>{category_value, category_value, {}};
    std::get<0>(temporary_user_info.spent_category[unused_name]) = std::get<0>(temporary_user_info.spent_category[unused_name]) - category_value;
    std::get<1>(temporary_user_info.spent_category[unused_name]) = std::get<0>(temporary_user_info.spent_category[unused_name]);


    // Update unused Series and slice
    auto donutBreakdown = static_cast<DonutBreakdownChart*>(ui->graphicsView->chart());
    donutBreakdown->mainSeries()->slices().first()->setValue(std::get<0>(temporary_user_info.spent_category[unused_name]));
    donutBreakdown->unusedMoneySeries()->slices().first()->setValue(std::get<0>(temporary_user_info.spent_category[unused_name]));
    // TODO double eq
    if(std::get<0>(temporary_user_info.spent_category[unused_name]) == 0.)
    {
        donutBreakdown->mainSeries()->slices().first()->setLabelVisible(false);
        donutBreakdown->unusedMoneySeries()->slices().first()->setLabelVisible(false);
    }

    // Create the series.
    auto series1 = new QPieSeries;
    series1->setName(QString::fromStdString(category_name));
    series1->append(QString::fromStdString(category_name), category_value);
    donutBreakdown->addBreakdownSeries(series1, getNextColorForNewSeries());

    donutBreakdown->recalculateAngles();
    donutBreakdown->updateLegendMarkers();
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
    temporary_user_info = {};   // reset the value before.
    temporary_user_info.monthlyIncome = monthly_revanue;
    temporary_user_info.spent_category[unused_name] = std::tuple<double, double, std::map<std::string,double>>{monthly_revanue, monthly_revanue, {}};

    auto donutBreakdown = new DonutBreakdownChart(nullptr, Qt::WindowFlags::fromInt(0) );
    donutBreakdown->setAnimationOptions(QChart::AllAnimations);
    donutBreakdown->legend()->setAlignment(Qt::AlignRight);
    
    ui->graphicsView->setChart(donutBreakdown);

    auto series_unused_money = new QPieSeries;
    series_unused_money->setName(QString::fromStdString(unused_name));
    series_unused_money->append(QString::fromStdString(unused_name), monthly_revanue);
    donutBreakdown->setUnusedMoneySeries(series_unused_money);
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


void StackedWindows::on_pushButtonMPRemoveSpendingCategoryOk_clicked()
{
    const std::string remove_category = ui->lineEditMPRemoveSpendingCategoryName->text().toStdString();
    if(temporary_user_info.spent_category.find(remove_category) == temporary_user_info.spent_category.end())
    {
        ui->labelMPRemoveSpendingCategoryWarning->setText("Category does not exist!");
        return;
    }
    if(remove_category == unused_name)
    {
        ui->labelMPRemoveSpendingCategoryWarning->setText("Cannot remove unused area!");
        return;
    }
    ui->labelMPRemoveSpendingCategoryWarning->setText("");

    // Update unused area

    std::get<0>(temporary_user_info.spent_category[unused_name]) = std::get<0>(temporary_user_info.spent_category[unused_name]) + std::get<0>(temporary_user_info.spent_category[remove_category]);
    std::get<1>(temporary_user_info.spent_category[unused_name]) = std::get<0>(temporary_user_info.spent_category[unused_name]);

    // Update unused Series and slice
    auto donutBreakdown = static_cast<DonutBreakdownChart*>(ui->graphicsView->chart());
    donutBreakdown->mainSeries()->slices().first()->setValue(std::get<0>(temporary_user_info.spent_category[unused_name]));
    donutBreakdown->unusedMoneySeries()->slices().first()->setValue(std::get<0>(temporary_user_info.spent_category[unused_name]));
    donutBreakdown->mainSeries()->slices().first()->setLabelVisible(true);
    donutBreakdown->unusedMoneySeries()->slices().first()->setLabelVisible(true);
    
    donutBreakdown->removeBreakdownSeries(remove_category);

    donutBreakdown->recalculateAngles();
    donutBreakdown->updateLegendMarkers();    

    temporary_user_info.spent_category.erase(remove_category);
}

