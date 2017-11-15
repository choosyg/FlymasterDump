#pragma once

#include <string>

template<class T>
T fromBytes( const char* b ){
    union Convert {
        char bytes[sizeof(T)];
        T value;
    };
    Convert c;
    for( size_t i=0; i<sizeof(T); ++i ){
        c.bytes[i] = b[i];
    }
    return c.value;
}

template< class T >
std::string toBytes( T v ){
    union Convert {
        char bytes[ sizeof(T) ];
        T value;
    };
    Convert c;
    c.value = v;
    return c.bytes;
}
