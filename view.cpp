#include "view.h"
#include "control.h"
#include <iostream>


extern QStringList EFFECTS_NAMES;
extern std::map<QString, Effect* > effects;
extern R_View View;
extern R_Model Model;


//init controllers array with arrays)))
R_View::R_View(){
    foreach( QString name, EFFECTS_NAMES )
        controllers[ name ] = new CONTROLLER_ARRAY;
}



//create layout and count controllers sizes
void R_View::init_effects_layout( QGroupBox * _group )
{
    layout = new QGridLayout( _group );
    QSize gr_size = _group->size();
    controller_size.setWidth( gr_size.width() / MAX_CONTROLLERS_IN_ROW - layout->horizontalSpacing() );
    controller_size.setHeight( gr_size.height() / MAX_CONTROLLERS_IN_COL - layout->verticalSpacing() );
}




//dispaly all controllers
void R_View::display_effect( QString effects_name )
{
    //get array(map) of controllers
    CONTROLLER_ARRAY* effect = controllers[ effects_name ];

    //kill children of layout. Wohahahahha!!!
    QList< QWidget* > children;
    do
    {
       children = layout->findChildren< QWidget* >();
       if ( children.count() == 0 )
           break;
       delete children.at( 0 );
    }
    while ( true );

    //just display every controller in the map
    for(auto& item : *effect )
    {
        Controller* cont = item.second;
        if( cont->type == "dial" )
            layout->addWidget( (QDial * )cont->widget );
        else if( cont->type == "combo" )
            layout->addWidget( (QComboBox * )cont->widget );
    }

    current_effect = effects_name;
}



//set minimum, maximum and notches for dials
Controller * R_View::new_dial( QString _name, void* _value, int _min, int _max ){

    //create usual dial
    QDial * dial = new QDial();
    dial->setMinimum( _min );
    dial->setMaximum( _max );
    dial->setNotchesVisible( true );
    dial->setFixedSize( controller_size );

    //create controller with needed presets and widget
    Controller * contr = new Controller( _name, _value, "dial", (void*)dial );

    //Connect signal and slot
    Controller::connect(
        (QObject*)dial,
        SIGNAL( valueChanged(int) ),
        contr,
        SLOT( changed( int ) )
    );

    return contr;
}




//constructor
Controller::Controller(QString _name, void *_value, QString _type, void*_widget)
{
    name = _name;
    value = _value;
    type = _type;
    widget = _widget;
}



//callback
void Controller::changed(int _value)
{
    float* dst = (float*)value;
    *dst = (float)_value;
    Model.refresh_effect( View.current_effect );
}



//PARAMS:
    //chorus: fWetDryMix, fDepth, fFeedback, fFrequency,lWaveform, fDelay, lPhase
    //compressor: fGain, fAttack, fRelease, fThreshold, fRatio, fPredelay
    //distortion: fGain, fEdge, fPostEQCenterFrequency, fPostEQBandwidth, fPreLowpassCutoff
    //echo: fWetDryMix, fFeedback, fLeftDelay, fRightDelay, lPanDelay
    //flanger: fWetDryMix, fDepth, fFeedback, fFrequency, lWaveform, fDelay, lPhase
    //gargle: dwRateHz, dwWaveShape
    //parameq: fCenter, fBandwidth, fGain
    //reverb: fInGain, fReverbMix, fReverbTime, fHighFreqRTRatio

void R_View::init( QGroupBox * _group )
{
    init_effects_layout( _group );

    //create all controllers
    BASS_DX8_CHORUS * param_chorus = (BASS_DX8_CHORUS*)effects["chorus"]->params;
    BASS_DX8_COMPRESSOR * param_compressor = (BASS_DX8_COMPRESSOR*)effects["compressor"]->params;
    BASS_DX8_DISTORTION * param_distortion = (BASS_DX8_DISTORTION*)effects["distortion"]->params;
    BASS_DX8_ECHO * param_echo = (BASS_DX8_ECHO*)effects["echo"]->params;
    BASS_DX8_FLANGER * param_flanger = (BASS_DX8_FLANGER*)effects["flanger"]->params;
    BASS_DX8_GARGLE * param_gargle = (BASS_DX8_GARGLE*)effects["gargle"]->params;
    BASS_DX8_PARAMEQ * param_parameq = (BASS_DX8_PARAMEQ*)effects["parameq"]->params;
    BASS_DX8_REVERB * param_reverb = (BASS_DX8_REVERB*)effects["reverb"]->params;

    //get pointer to distortion params
    CONTROLLER_ARRAY * distortion = controllers[ "distortion" ];
    (*distortion)[ "fGain" ] = new_dial( "Gain", &(param_distortion->fGain), -60, 0 );
}



