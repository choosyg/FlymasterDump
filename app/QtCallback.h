#pragma once

#ifndef Q_MOC_RUN
#include <progress/Callback.h>
#endif

#include <QObject>

class QtCallback : public QObject, public Callback {
    Q_OBJECT
public:
    QtCallback( QObject* parent = 0 );
    void notify( Severity s, const std::string& m ) override;
    void progress( unsigned short percent ) override;
    bool isCancelled() const override;

    void cancel();

    size_t count( Severity s ) const;
    QStringList messages( Severity s ) const;
    void reset();
signals:
    void notified( Severity, QString );
    void progressChanged( unsigned int );

private:
    bool isCancelled_ = false;

    std::map< Severity, QStringList > messages_;
};
