#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowelements.cpp"
#include "asiocontrol.h"

QString current_effect = "distortion";
extern R_View View;
extern R_Model Model;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    View.init( ui->effect_group );
    CONTROLLER_ARRAY* dist = View.controllers[ "distortion" ];
    WindowElements::create_logo( ui->label );
    WindowElements::create_dial_group();

    View.display_effect( "distortion" );
}

MainWindow::~MainWindow()
{
    Model.asio_dispose();
    delete ui;
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if( checked ){
        //effect is switched on
        Model.set_effect( current_effect );
    }
    else{
        //effect is switched off
        Model.remove_effect( current_effect );
    }
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString name( arg1 );
    View.current_effect = name;
    View.display_effect( name );
}
