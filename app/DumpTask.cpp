#include "DumpTask.h"

#include <serialPortCom/SerialPort.h>
#include <serialPortCom/Request.h>
#include <serialPortCom/IgcFactory.h>

#include <fstream>

DumpTask::DumpTask(QString dir, FlightMetaInfo info, Model model)
    : Task( QObject::tr("Requesing flight #%1").arg(info.index) ),
      dir_(dir),
      info_(info),
      model_(model)
{
}

void DumpTask::run()
{
    SerialPort port( model_.serialPort().toStdString() );
    port.setSpeed( model_.serialPortSpeed() );
    auto bfd = requestFlight( port, info_, callback() );
    if( callback().count( Severity::Critical ) == 0  ){
        auto igc = buildIgc( info_, bfd );
        igc.hRecord.pilot = model_.pilot().toStdString();
        igc.hRecord.gliderType = model_.glider().toStdString();
        igc.hRecord.gliderId = model_.gliderId().toStdString();
        igc.hRecord.contest = model_.contest().toStdString();
        igc.hRecord.site = model_.site().toStdString();

        std::string file = dir_.toStdString()
                + "/" + info_.date.year
                + "-" + info_.date.month
                + "-" + info_.date.day
                + " UTC"
                +info_.time.hours
                +info_.time.minutes
                +info_.time.seconds
                + ".igc";
        callback().notify( Severity::Info, "Storing IGC File: " + file );
        std::ofstream os( file );
        os << igc.build();
        os.flush();
        os.close();
    }
}

FlightMetaInfo DumpTask::info() const
{
    return info_;
}
