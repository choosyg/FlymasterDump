#include "QtCallback.h"

QtCallback::QtCallback( QObject* parent ) : QObject( parent ) {
}

void QtCallback::notify( Severity s, const std::string& m ) {
    emit notified( s, m.c_str() );
}

void QtCallback::progress( unsigned short percent ) {
    emit progressChanged( percent );
}
