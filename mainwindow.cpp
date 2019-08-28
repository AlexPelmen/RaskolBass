#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowelements.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WindowElements::create_logo( ui->label );
    WindowElements::create_dial_group( ui->effect_group );

}

MainWindow::~MainWindow()
{
    delete ui;
}
