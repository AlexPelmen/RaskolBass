#include "asiocontrol.h"
#include "bass.h"
#include "bassasio.h"
#include <iostream>


DWORD AsioControl::inputProc(BOOL input, DWORD channel, void *buffer, DWORD length, void *user){
    BASS_StreamPutData(input_signal, buffer, length);
    return length;
}


void AsioControl::init_asio(){
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


void AsioControl::asio_dispose(){
    BASS_ASIO_Stop();
    BASS_ASIO_Free();
    BASS_Stop();
    BASS_Free();
}


