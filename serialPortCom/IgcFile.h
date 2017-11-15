#pragma once

#include <string>
#include <vector>

template < size_t n > class FixedSizeString {
public:
    FixedSizeString() {
        for( size_t i = 0; i < n; ++i ) {
            str_[i] = ' ';
        }
        str_[n] = '\0';
    }

    FixedSizeString( const char str[n] ) {
        for( size_t i = 0; i < n; ++i ) {
            str_[i] = str[i];
        }
        str_[n] = '\0';
    }

    size_t size() const {
        return n;
    }

    operator std::string() const {
        return std::string( &str_[0] );
    }

    operator char*() {
        return &str_[0];
    }

    char& operator[]( size_t i ) {
        return str_[i];
    }

private:
    char str_[n + 1];
};

class ARecord {
public:
    FixedSizeString< 3 > manufacturer;
    FixedSizeString< 3 > uid;
    std::string id;
    std::string build() const;
};

class BRecord {
public:
    FixedSizeString< 2 > hour;
    FixedSizeString< 2 > minutes;
    FixedSizeString< 2 > seconds;
    FixedSizeString< 8 > latitude;      // DDMMmmmN/S
    FixedSizeString< 9 > longitude;     // DDDMMmmmE/W
    FixedSizeString< 1 > validAltitude; // A=3DFix / V=2DFix
    FixedSizeString< 5 > altitudePressure;
    FixedSizeString< 5 > altitudeGPS;
    std::string build() const;
};

class HRecord {
public:
    FixedSizeString< 6 > flightDate;
    std::string firmwareVersion;
    std::string hardwareVersion;
    std::string pilot;
    std::string gliderType;
    std::string gliderId;
    std::string contest;
    std::string site;
    std::string build() const;
};

class IgcFile {
public:
    ARecord aRecord;
    HRecord hRecord;
    std::vector< BRecord > bRecords;

    std::string build() const;
};
