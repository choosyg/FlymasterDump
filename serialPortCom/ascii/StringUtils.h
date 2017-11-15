#pragma once

#include <string>
#include <vector>

typedef std::vector< std::string > StringList;
StringList split( const std::string& str, char delimiter );

std::string join( const StringList& list, char delimiter );
