#include "IgcFile.h"

std::string ARecord::build() const {
    std::string str = "A";
    str += manufacturer;
    str += uid;
    str += id;
    return str + "\n";
}

std::string BRecord::build() const {
    std::string str = "B";
    str += hour;
    str += minutes;
    str += seconds;
    str += latitude;
    str += longitude;
    str += validAltitude;
    str += altitudePressure;
    str += altitudeGPS;
    return str + "\n";
}

std::string HRecord::build() const {
    std::string str;
    str += "HFDTE" + static_cast< std::string >( flightDate ) + "\n";
    str += "HFPLTPILOTINCHARGE: " + pilot + "\n";
    str += "HFGTYGLIDERTYPE: " + gliderType + "\n";
    str += "HFGIDGLIDERID: " + gliderId + "\n";
    str += "HFCIDCOMPETITIONID: " + contest + "\n";
    str += "HFDTMGPSDATUM: WGS84\n";
    str += "HFRFWFIRMWAREVERSION: " + firmwareVersion + "\n";
    str += "HFRHWHARDWAREVERSION: " + hardwareVersion + "\n";
    str += "HOSITSite: " + site + "\n";
    return str;
}

std::string IgcFile::build() const {
    std::string str = aRecord.build();
    str += hRecord.build();

    for( const auto& b : bRecords ) {
        str += b.build();
    }

    return str;
}
