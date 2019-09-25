#include "view.h"
#include "control.h"
#include <iostream>
#include <QGroupBox>


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
    QObjectList childs = layout->children();
    for (int i=0; i< childs.size(); i++)
    {
        delete childs[i];
    }


    //just display every layout with controller in the map
    int row = 0;
    int col = 0;
    for(auto& item : *effect )
    {
        Controller* cont = item.second;
        QVBoxLayout * inner_layout = new QVBoxLayout();

        if( cont->type == "dial" )
            inner_layout->addWidget( (QDial * )cont->widget );
        else if( cont->type == "combo" )
            inner_layout->addWidget( (QComboBox * )cont->widget );

        QGroupBox * box = new QGroupBox( cont->name );
        box->setLayout( inner_layout );

        //output box
        layout->addWidget( box, col, row);
        if( ++row > MAX_CONTROLLERS_IN_COL ){
            row = 0;
            ++col;
        }
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
    //dial->setFixedSize( controller_size );

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

    //get all pointers to BASS_DX8 structures
    BASS_DX8_CHORUS * param_chorus = (BASS_DX8_CHORUS*)effects["chorus"]->params;
    BASS_DX8_COMPRESSOR * param_compressor = (BASS_DX8_COMPRESSOR*)effects["compressor"]->params;
    BASS_DX8_DISTORTION * param_distortion = (BASS_DX8_DISTORTION*)effects["distortion"]->params;
    BASS_DX8_ECHO * param_echo = (BASS_DX8_ECHO*)effects["echo"]->params;
    BASS_DX8_FLANGER * param_flanger = (BASS_DX8_FLANGER*)effects["flanger"]->params;
    BASS_DX8_GARGLE * param_gargle = (BASS_DX8_GARGLE*)effects["gargle"]->params;
    BASS_DX8_PARAMEQ * param_parameq = (BASS_DX8_PARAMEQ*)effects["parameq"]->params;
    BASS_DX8_REVERB * param_reverb = (BASS_DX8_REVERB*)effects["reverb"]->params;

    //CHORUS
    CONTROLLER_ARRAY * chorus = controllers[ "chorus" ];
    (*chorus)[ "fWetDryMix" ] = new_dial( "WetDryMix", &(param_chorus->fWetDryMix ), 0, 100 );
    (*chorus)[ "fDepth" ] = new_dial( "Depth", &(param_chorus->fDepth),  0, 100 );
    (*chorus)[ "fFeedback" ] = new_dial( "Feedback", &(param_chorus->fFeedback), -99, 99 );
    (*chorus)[ "fFrequency" ] = new_dial( "Frequency", &(param_chorus->fFrequency), 0, 10 );
    (*chorus)[ "lWaveform" ] = new_dial( "Waveform", &(param_chorus->lWaveform), 0, 1 ); //todo
    (*chorus)[ "fDelay" ] = new_dial( "Delay", &(param_chorus->fDelay), 0, 20 );
    (*chorus)[ "lPhase" ] = new_dial( "Phase", &(param_chorus->lPhase), 0, 10 );    //todo

    //COMPRESSOR
    CONTROLLER_ARRAY * compressor = controllers[ "compressor" ];
    (*compressor)["fGain"] = new_dial( "fGain", &(param_compressor->fGain ), -60, 60 );
    (*compressor)["fAttack"] = new_dial( "fAttack", &(param_compressor->fAttack ), 0.01, 500 );
    (*compressor)["fRelease"] = new_dial( "fRelease", &(param_compressor->fRelease ), 50, 3000 );
    (*compressor)["fThreshold"] = new_dial( "fThreshold", &(param_compressor->fThreshold ), -60, 0 );
    (*compressor)["fRatio"] = new_dial( "fRatio", &(param_compressor->fRatio ), 1, 100 );
    (*compressor)["fPredelay"] = new_dial( "fPredelay", &(param_compressor->fPredelay ), 0, 4 );

    //DISTORTION
    CONTROLLER_ARRAY * distortion = controllers[ "distortion" ];
    (*distortion)[ "fGain" ] = new_dial( "Gain", &(param_distortion->fGain), -60, 0 );
    (*distortion)[ "fEdge" ] = new_dial( "Edge", &(param_distortion->fGain), -60, 0 );
    (*distortion)[ "fPostEQCenterFrequency" ] = new_dial( "CenterFreq", &(param_distortion->fGain), -60, 0 );
    (*distortion)[ "fPostEQBandwidth" ] = new_dial( "Bandwidth", &(param_distortion->fGain), -60, 0 );
    (*distortion)[ "fPreLowpassCutoff" ] = new_dial( "Cutoff", &(param_distortion->fGain), -60, 0 );

    //ECHO
    CONTROLLER_ARRAY * echo = controllers[ "echo" ];
    (*echo)["fWetDryMix"] = new_dial( "fWetDryMix", &(param_echo->fWetDryMix), 0, 100 );
    (*echo)["fFeedback"] = new_dial( "fFeedback", &(param_echo->fFeedback),  0, 100);
    (*echo)["fLeftDelay"] = new_dial( "fLeftDelay", &(param_echo->fLeftDelay), 1, 2000 );
    (*echo)["fRightDelay"] = new_dial( "fRightDelay", &(param_echo->fRightDelay), 1,2000 );
    (*echo)["lPanDelay"] = new_dial( "lPanDelay", &(param_echo->lPanDelay), 0, 1 );     //todo

    //FLANGER
    CONTROLLER_ARRAY * flanger = controllers[ "flanger" ];
    (*flanger)["fWetDryMix"] = new_dial( "fWetDryMix", &(param_flanger->fWetDryMix), 0, 100 );
    (*flanger)["fDepth"] = new_dial( "fDepth", &(param_flanger->fDepth), 0, 100 );
    (*flanger)["fFeedback"] = new_dial( "fFeedback", &(param_flanger->fFeedback), -99, 99 );
    (*flanger)["fFrequency"] = new_dial( "fFrequency", &(param_flanger->fFrequency), 0, 10 );
    (*flanger)["lWaveform"] = new_dial( "lWaveform", &(param_flanger->lWaveform), 0, 1 );
    (*flanger)["fDelay"] = new_dial( "fDelay", &(param_flanger->fDelay), 0, 20 );
    (*flanger)["lPhase"] = new_dial( "lPhase", &(param_flanger->lPhase), 0, 10 );

    //GARGLE
    CONTROLLER_ARRAY * gargle = controllers[ "gargle" ];
    (*gargle)["dwRateHz"] = new_dial( "dwRateHz", &(param_gargle->dwRateHz), 1, 1000 );
    (*gargle)["dwWaveShape"] = new_dial( "dwRateHz", &(param_gargle->dwRateHz), 0, 1 ); //todo

    CONTROLLER_ARRAY * parameq = controllers[ "parameq" ];
    (*gargle)["fCenter"] = new_dial( "fCenter", &(param_parameq->fCenter), 80, 16000 );
    (*gargle)["fBandwidth"] = new_dial( "fBandwidth", &(param_parameq->fBandwidth), 1, 36 );
    (*gargle)["fGain"] = new_dial( "fGain", &(param_parameq->fGain), -15, 15 );

    CONTROLLER_ARRAY * reverb = controllers[ "reverb" ];
    (*reverb)["fInGain"] = new_dial( "fInGain", &(param_reverb->fInGain), -96, 0 );
    (*reverb)["fReverbMix"] = new_dial( "fReverbMix", &(param_reverb->fReverbMix), -96, 0 );
    (*reverb)["fReverbTime"] = new_dial( "fReverbTime", &(param_reverb->fReverbTime), 0.001, 3000 );
    (*reverb)["fHighFreqRTRatio"] = new_dial( "fHighFreqRTRatio", &(param_reverb->fHighFreqRTRatio), 0.001, 0.999 );
}



