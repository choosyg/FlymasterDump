#pragma once

#include "NmeaSentence.h"

struct Date {
    std::string year;
    std::string month;
    std::string day;
};

struct Time {
    std::string hours;
    std::string minutes;
    std::string seconds;
};

class FlightMetaInfo {
public:
    FlightMetaInfo( const FlightMetaInfo& ) = default;
    FlightMetaInfo( const NmeaSentence& sentence );

    int index;
    Date date;
    Time time;
    Time duration;

    std::string flightRequest() const;
};
