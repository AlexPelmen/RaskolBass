#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QDial>

namespace WindowElements{

    //create logo to the window
    void create_logo( QLabel * label ){
        QPixmap pix = QPixmap("c:/Users/Pelmen/Documents/RaskolBass/pic/logo.png");
        pix = pix.scaled( label->width(), label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap( pix );
    }


    //create dial group
    void create_dial_group( QGroupBox * effect_group ){
        //dial params
        int gr_w = effect_group->width();
        int gr_h = effect_group->height();

        QGridLayout *effect_layout = new QGridLayout( effect_group );
        int sp_h = effect_layout->horizontalSpacing();
        int sp_v = effect_layout->verticalSpacing();
        int row_num = 2;
        int col_num = 5;

        int dial_width = gr_w / col_num - sp_h;
        int dial_height = gr_h / row_num - sp_v;

        for( int i = 0; i < row_num; i++ ){
            for( int j = 0; j < col_num; j++ ){
                QDial *dial = new QDial();
                dial->setFixedSize( dial_width, dial_height );
                dial->setNotchesVisible( true );
                effect_layout->addWidget( dial, i, j );
            }
        }
    }


    //create combobox
    void create_combobox(){

    }

}
