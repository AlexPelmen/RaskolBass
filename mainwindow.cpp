#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDial>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //logo
    QPixmap pix = QPixmap("c:/Users/Pelmen/Documents/RaskolBass/pic/logo.png");
    pix = pix.scaled( ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap( pix );

    //dial params
    int gr_w = ui->effect_group->width();
    int gr_h = ui->effect_group->height();
    int sp_h = ui->effect_layout->horizontalSpacing();
    int sp_v = ui->effect_layout->verticalSpacing();
    int row_num = 2;
    int col_num = 5;

    int dial_width = gr_w / col_num - sp_h;
    int dial_height = gr_h / row_num - sp_v;

    for( int i = 0; i < row_num; i++ ){
        for( int j = 0; j < col_num; j++ ){
            QDial *dial = new QDial();
            dial->setFixedSize( dial_width, dial_height );
            dial->setNotchesVisible( true );
            ui->effect_layout->addWidget( dial, i, j );
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
