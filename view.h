#ifndef VIEW_H
#define VIEW_H

#include "asiocontrol.h"
#include <QString>
#include <QCombobox>
#include <QDial>
#include <QGridLayout>
#include <QGroupBox>
#include <QSize>
#include <map>
#include <QWidget>

#define MAX_CONTROLLERS_IN_ROW 5
#define MAX_CONTROLLERS_IN_COL 2

#define CHORUS_CONTROLLERS_NUM 7
#define COMPRESSOR_CONTROLLERS_NUM 6
#define DISTORTION_CONTROLLERS_NUM 5
#define ECHO_CONTROLLERS_NUM 5
#define FLANGER_CONTROLLERS_NUM 7
#define GARGLE_CONTROLLERS_NUM 2
#define PARAMEQ_CONTROLLERS_NUM 3
#define REVERB_CONTROLLERS_NUM 4


//class for two controllers: dial and combobox
class Controller: public QObject{

    Q_OBJECT

public:
    Controller( QString _name, void* _value, QString _type, void* _widget );
    QString name;
    void* value;
    QString type;
    void* widget;

public:
    void display();

public slots:
    void changed( int value );
};

//New type controller array
typedef std::map<QString, Controller*> CONTROLLER_ARRAY;


class R_View
{
public:
    QGridLayout * layout;
    QGroupBox * group;
    QSize controller_size;
    QString current_effect;

    std::map< QString, CONTROLLER_ARRAY* > controllers;



public:
    R_View();
    void init( QGroupBox * _group );
    void init_effects_layout( QGroupBox * _group );
    void show_effect( QString effects_name );

    Controller* new_dial( QString _name, void* _value, int _min, int _max );  
};

#endif // VIEW_H
