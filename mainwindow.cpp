#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix = QPixmap("c:/Users/Pelmen/Documents/RaskolBass/pic/logo.png");
    pix = pix.scaled( ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap( pix );
}

MainWindow::~MainWindow()
{
    delete ui;
}
