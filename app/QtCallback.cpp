#include "QtCallback.h"

QtCallback::QtCallback( QObject* parent ) : QObject( parent ) {
    qRegisterMetaType<Severity>("Severity");
}

void QtCallback::notify( Severity s, const std::string& m ) {
    emit notified( s, m.c_str() );
}

void QtCallback::progress( unsigned short percent ) {
    emit progressChanged( percent );
}

bool QtCallback::isCancelled() const
{
    return isCancelled_;
}

void QtCallback::cancel()
{
    isCancelled_ = true;
}

void QtCallback::setCancelled(bool b)
{
    isCancelled_ = b;
}
