#include "QtCallback.h"

QtCallback::QtCallback( QObject* parent ) : QObject( parent ) {
    qRegisterMetaType<Severity>("Severity");
}

void QtCallback::notify( Severity s, const std::string& m ) {
    messages_[s].append( m.c_str() );
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

size_t QtCallback::count(Severity s) const
{
    return messages( s ).size();
}

QStringList QtCallback::messages(Severity s) const
{
    auto it = messages_.find( s );
    if( it == messages_.end() ){
        return QStringList();
    }
    return it->second;
}

void QtCallback::reset()
{
    isCancelled_ = false;
    messages_.clear();
}
