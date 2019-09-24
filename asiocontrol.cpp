#include "asiocontrol.h"
#include "bass.h"
#include "bassasio.h"
#include <iostream>

extern QStringList EFFECTS_NAMES;
extern std::map<QString, Effect*> effects;

//constructor
R_Model::R_Model(){
    init_effects();
}

DWORD R_Model::inputProc(BOOL input, DWORD channel, void *buffer, DWORD length, void *user){
    BASS_StreamPutData(input_signal, buffer, length);
    return length;
}


void R_Model::init_asio(){
    //Initialize Bass
    try {
        if ( ! BASS_Init( 1, 44100, 0, 0, NULL) )
            throw BASS_ErrorGetCode();
        if ( ! BASS_ASIO_Init( 0, NULL ) )
            throw BASS_ASIO_ErrorGetCode();
    }
    catch ( int err ) {
        std::cout << "Err no - " << err << std::endl;
        system("pause");
        return;
    }

    input_signal = BASS_StreamCreate(BASS_ASIO_GetRate(), 1, BASS_STREAM_DECODE, STREAMPROC_PUSH, 0);
    BASS_ASIO_ChannelEnable(TRUE, 0, inputProc, 0);
    BASS_ASIO_ChannelEnableBASS(FALSE, 0, input_signal, TRUE);
    BASS_ASIO_ChannelEnableBASS(FALSE, 1, input_signal, TRUE);

    BASS_ASIO_ChannelSetFormat(TRUE, 0, BASS_ASIO_FORMAT_16BIT);
    BASS_ASIO_ChannelSetFormat(FALSE, 0, BASS_ASIO_FORMAT_16BIT );
    BASS_ASIO_ChannelSetFormat(FALSE, 1, BASS_ASIO_FORMAT_16BIT );

    BASS_ASIO_ChannelEnableMirror( 1, 0, 0 );
    BASS_ASIO_Start(0, 0);
}


void R_Model::asio_dispose(){
    BASS_ASIO_Stop();
    BASS_ASIO_Free();
    BASS_Stop();
    BASS_Free();
}

//initialize effects array
void R_Model::init_effects()
{
    effects[ "chorus" ] = new Effect( BASS_FX_DX8_CHORUS, new BASS_DX8_CHORUS );
    effects[ "compressor" ] = new Effect( BASS_FX_DX8_COMPRESSOR, new BASS_DX8_COMPRESSOR );
    effects[ "distortion" ] = new Effect( BASS_FX_DX8_DISTORTION, new BASS_DX8_DISTORTION );
    effects[ "echo" ] = new Effect( BASS_FX_DX8_ECHO, new BASS_DX8_ECHO );
    effects[ "flanger" ] = new Effect( BASS_FX_DX8_FLANGER, new BASS_DX8_FLANGER );
    effects[ "gargle" ] = new Effect( BASS_FX_DX8_GARGLE, new BASS_DX8_GARGLE );
    effects[ "parameq" ] = new Effect( BASS_FX_DX8_PARAMEQ, new BASS_DX8_PARAMEQ );
    effects[ "reverb" ] = new Effect( BASS_FX_DX8_REVERB, new BASS_DX8_REVERB );
}

void R_Model::set_effect( QString name ){
    Effect *eff = effects[ name ];
    eff->handle = BASS_ChannelSetFX( input_signal, EFFECTS_NAMES.indexOf( name ), 1 );
    eff->active = true;
}

void R_Model::remove_effect( QString name ){
    Effect *eff = effects[ name ];
    eff->active = false;
    BASS_ChannelRemoveFX( input_signal, eff->handle );
}

void R_Model::refresh_effect(QString name){
    Effect* eff = effects[ name ];
    BASS_FXSetParameters( eff->handle, eff->params );
}


