#include "FlightListTask.h"

#include <serialPortCom/Request.h>

FlightListTask::FlightListTask(Model model) :model_(model){}

void FlightListTask::run()
{
    SerialPort port( model_.serialPort().toStdString() );
    port.setSpeed( model_.serialPortSpeed() );
    infos_ = requestFlightInfos( port, callback() );
    for( const auto& info : infos_ ) {
        QString flight = QString( "%1: %2-%3-%4 %5:%6:%7 Duration: %8:%9:%10" )
                .arg( info.index )
                .arg( info.date.year.c_str() )
                .arg( info.date.month.c_str() )
                .arg( info.date.day.c_str() )
                .arg( info.time.hours.c_str() )
                .arg( info.time.minutes.c_str() )
                .arg( info.time.seconds.c_str() )
                .arg( info.duration.hours.c_str() )
                .arg( info.duration.minutes.c_str() )
                .arg( info.duration.seconds.c_str() );
        list_.append( flight );
    }
}

const QStringList &FlightListTask::list() const
{
    return list_;
}

const std::vector<FlightMetaInfo> &FlightListTask::infos() const
{
    return infos_;
}
