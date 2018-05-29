#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tuple>
#include <QDebug>
#include <qfile.h>
#include <widgetypes.h>
#include <map>
#include <cint.h>
#include <enum.h>
#include "fileops.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    WidgetTypes widget_types(std::make_tuple(

                cint<TickTypeDef::Active>{}, "Activated", "A",
                cint<TickTypeDef::Deactive>{}, "Deactivated", "D",

                cint<ColorTypeDef::Blue>{}, "Blue", "B",
                cint<ColorTypeDef::Orange>{}, "Orange", "O",
                cint<ColorTypeDef::Red>{}, "Red", "R",
                cint<ColorTypeDef::Green>{}, "Green", "G"

                ));

    std::string data;

    ui->setupUi(this);
    ColorTypeDef Colors;
    TickTypeDef ticks;


    widget_types.setPersistancy(std::make_tuple(
                          ui->comboBox, Colors,
                          ui->checkBox, ticks,
                          ui->comboBox, ui->checkBox
                          )
                );

    widget_types = ColorTypeDef::Blue;

    std::vector<std::string> vec = widget_types;

    for(int i=0; i<vec.size(); i++)
        std::cout<<vec[i] <<std::endl;

    std::map<int, std::string> M = widget_types;



    std::cout << M[ColorTypeDef::Green] << std::endl;

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

    if(ui->comboBox->currentText() == "Green")
        this->setStyleSheet("background-color: Green;");
    if(ui->comboBox->currentText() == "Red")
        this->setStyleSheet("background-color: Red;");
    if(ui->comboBox->currentText() == "Blue")
        this->setStyleSheet("background-color: Blue;");
    if(ui->comboBox->currentText() == "Orange")
        this->setStyleSheet("background-color: Orange;");

}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

    if(ui->checkBox->checkState() == Qt::CheckState::Checked)
        ui->checkBox->setText("Activate");
    if(ui->checkBox->checkState() == Qt::CheckState::Unchecked)
        ui->checkBox->setText("Deactivate");

}
