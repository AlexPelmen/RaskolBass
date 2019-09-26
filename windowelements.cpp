#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QDial>
#include "view.h"

extern R_View View;
extern QStringList EFFECTS_NAMES;

namespace WindowElements{

    //create logo to the window
    void create_logo( QLabel * label ){
        QPixmap pix = QPixmap("c:/Users/Pelmen/Documents/RaskolBass/pic/logo.png");
        pix = pix.scaled( label->width(), label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap( pix );
    }


    //create dial group
    void create_dial_group(){
        View.show_effect( "distortion" );
    }

    //create combobox
    void create_combobox( QComboBox * combo ){
        foreach( QString name, EFFECTS_NAMES )
            combo->addItem( name );
    }

}
