#pragma once

#include <string>
#include <vector>

/// A string of the form $Command,comma,separated,values*checksum
class NmeaSentence {
public:
    NmeaSentence() = default;
    NmeaSentence( const std::string& sentence );

    std::string command;
    std::vector< std::string > values;

    std::string build() const;

private:
    int calculateChecksum( const std::string& str ) const;

    short parseChecksum( const std::string& cs ) const;
    std::string printChecksum( short s ) const;
    std::string checkAndStrip( const std::string& sentence );
};
