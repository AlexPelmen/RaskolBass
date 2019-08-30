#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowelements.cpp"
#include "asiocontrol.h"

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
    AsioControl::asio_dispose();
    delete ui;
}
