#ifndef ASIOCONTROL_H
#define ASIOCONTROL_H

#include "bass.h"
#include "bassasio.h"
#include <map>
#include <QStringList>


//standart effect class
class Effect{
public:
    Effect( int _type, void* _params ){ type = _type; params = _params; active = false; }
    void* params;
    int type;
    HFX handle;
    BOOL active;
};


class R_Model{
public:
    R_Model();
    //Stream for the input signal
    static HSTREAM input_signal;

    //Callback for input data from asio driver
    static DWORD CALLBACK inputProc(BOOL input, DWORD channel, void *buffer, DWORD length, void *user);

    //init asio driver
    void init_asio();

    //release memory and stop driver
    void asio_dispose();

    void init_effects();
    Effect* get_effect(QString name );
    void set_effect( QString name );
    void remove_effect( QString name );
    void refresh_effect( QString name );
};

#endif // ASIOCONTROL_H
