#ifndef ASIOCONTROL_H
#define ASIOCONTROL_H

#include "bass.h"
#include "bassasio.h"

class AsioControl{
public:
    //Stream for the input signal
    static HSTREAM input_signal;

    //Callback for input data from asio driver
    static DWORD CALLBACK inputProc(BOOL input, DWORD channel, void *buffer, DWORD length, void *user);

    //init asio driver
    static void init_asio();

    //release memory and stop driver
    static void asio_dispose();
};

#endif // ASIOCONTROL_H
