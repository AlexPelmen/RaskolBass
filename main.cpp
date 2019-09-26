#include "mainwindow.h"
#include "asiocontrol.h"
#include <QApplication>
#include <tuple>
#include "view.h"
#include "control.h"

QStringList EFFECTS_NAMES = (QStringList() << "chorus" << "compressor" << "distortion" << "echo" << "flanger" << "gargle" << "parameq" << "reverb" );
HSTREAM R_Model::input_signal;
std::map<QString,Effect*> effects;
R_Model Model;
R_View View;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Model.init_asio();
    Model.set_effect( "distortion" );
    Model.refresh_effect( "distortion" );


    return a.exec();
}
