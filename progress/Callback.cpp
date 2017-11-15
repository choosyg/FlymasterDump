#include "Callback.h"

#include <iostream>

void Callback::notify( Severity s, const std::string& m ) {
}

void Callback::progress( unsigned short percent ) {
}

bool Callback::isCancelled() const {
    return false;
}

void PrintCallback::notify( Severity s, const std::string& m ) {
    switch( s ) {
        case Severity::Debug:
            std::cout << "[DEBUG]   ";
            break;
        case Severity::Info:
            std::cout << "[INFO]    ";
            break;
        case Severity::Warning:
            std::cout << "[WARNING] ";
            break;
        case Severity::Error:
            std::cout << "[ERROR]   ";
            break;
        case Severity::Critical:
            std::cout << "[CRITICAL]";
            break;
        default:
            break;
    }
    std::cout << m << std::endl;
}

void PrintCallback::progress( unsigned short percent ) {
    std::cout << "[PROGRESS] " << percent << std::endl;
}
