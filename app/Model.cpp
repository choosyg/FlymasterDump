#include "Model.h"

#include <QSettings>

void persist(const Model &model){
    QSettings settings( "settings.ini", QSettings::IniFormat );

    settings.beginGroup( "Config" );
    settings.setValue( "Port", model.serialPort() );
    settings.setValue( "Speed", static_cast<int>( model.serialPortSpeed() ) );
    settings.setValue( "RecentSaveDir", model.recentSaveDir() );
    settings.endGroup();

    settings.beginGroup( "Info" );
    settings.setValue( "Pilot", model.pilot() );
    settings.setValue( "PilotID", model.pilotId() );
    settings.setValue( "Glider", model.glider() );
    settings.setValue( "GliderSerial", model.gliderSerial() );
    settings.setValue( "Site", model.site() );
    settings.setValue( "Contest", model.contest() );
    settings.endGroup();
}

void load(Model &model){
    QSettings settings( "settings.ini", QSettings::IniFormat );

    settings.beginGroup( "Config" );
    model.serialPort = settings.value( "Port" ).toString();
    model.serialPortSpeed = static_cast<Speed>( settings.value( "Speed" ).toInt() );
    model.recentSaveDir = settings.value( "RecentSaveDir" ).toString();
    settings.endGroup();

    settings.beginGroup( "Info" );
    model.pilot = settings.value( "Pilot" ).toString();
    model.pilotId = settings.value( "PilotID" ).toString();
    model.glider = settings.value( "Glider" ).toString();
    model.gliderSerial = settings.value( "GliderSerial" ).toString();
    model.site = settings.value( "Site" ).toString();
    model.contest = settings.value( "Contest" ).toString();
    settings.endGroup();
}
