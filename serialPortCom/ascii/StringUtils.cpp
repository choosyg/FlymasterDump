#include "StringUtils.h"

#include <sstream>

StringList split( const std::string& str, char delimiter ) {
    std::vector< std::string > elements;
    std::stringstream ss( str );
    std::string item;
    while( std::getline( ss, item, delimiter ) ) {
        elements.push_back( item );
    }
    if( str.back() == delimiter ) {
        elements.emplace_back( "" );
    }
    return elements;
}

std::string join( const StringList& list, char delimiter ) {
    if( list.empty() ) {
        return "";
    }

    std::string str;
    for( const auto& item : list ) {
        str += item + delimiter;
    }
    return str.substr( 0, str.size() - 1 );
}
