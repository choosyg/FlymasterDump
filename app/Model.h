#pragma once

#include "Property.h"

#include <serialPortCom/SerialPort.h>

#include <QDir>

class Model{
public:
    Property< QString > serialPort;
    Property< Speed > serialPortSpeed;

    Property< QString > recentSaveDir;

    Property< QString > pilot;
    Property< QString > pilotId;
    Property< QString > glider;
    Property< QString > gliderSerial;
    Property< QString > site;
    Property< QString > contest;
};

void persist( const Model& model );
void load( Model& model );
