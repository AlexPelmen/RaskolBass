#include "mainwindow.h"
#include "asiocontrol.h"
#include <QApplication>

HSTREAM AsioControl::input_signal;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    AsioControl::init_asio();

    return a.exec();
}
