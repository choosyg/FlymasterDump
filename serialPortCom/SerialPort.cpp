#include "SerialPort.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <boost/bind.hpp>

using namespace boost::asio;

SerialPort::SerialPort( const std::string& port ) : port_( ioService_ ), timeout_( ioService_ ), timeoutMs_( 100 ) {
    boost::system::error_code ec;
    port_.open( port );
    if( !port_.is_open() ) {
        throw std::runtime_error( "Failed to connect to COM3: " + ec.message() );
    }
    setSpeed( Speed::BAUD115200 );
}

SerialPort::~SerialPort() {
    port_.close();
}

void SerialPort::setSpeed( Speed s ) {
    port_.set_option( serial_port_base::baud_rate( static_cast< unsigned int >( s ) ) );
}

void SerialPort::setTimeout( unsigned int ms ) {
    timeoutMs_ = ms;
}

std::string SerialPort::request( const std::string& request ) {
    writeRequest( request );
    return readResponse();
}

void SerialPort::writeRequest( const std::string& request ) {
    streambuf requestBuffer;
    std::ostream requestStream( &requestBuffer );
    requestStream << request << "\n";
    write( port_, requestBuffer );
}

std::string SerialPort::readResponse() {
    response_ = "";
    dataAvailable_ = false;
    buffer_[0] = '\0';
    port_.get_io_service().reset();
    port_.async_read_some( buffer( buffer_, 255 ),
                           boost::bind( &SerialPort::read_callback,
                                        this,
                                        placeholders::error,
                                        placeholders::bytes_transferred ) );
    timeout_.expires_from_now( boost::posix_time::milliseconds( timeoutMs_ ) );
    timeout_.async_wait( boost::bind( &SerialPort::wait_callback, this, placeholders::error ) );

    ioService_.run(); // will block until async callbacks are finished

    return response_;
}

void SerialPort::read_callback( const boost::system::error_code& error, size_t bytes_transferred ) {
    if( error && error != boost::asio::error::operation_aborted || !bytes_transferred ) {
        dataAvailable_ = false;
        return;
    }

    timeout_.cancel();

    for( size_t i = 0; i < bytes_transferred; ++i ) { //+2 because it ends with \r\n
        if( buffer_[i] == '\r' ) {
            continue;
        }
        response_ += buffer_[i];
    }
    buffer_[0] = '\0';

    port_.async_read_some( buffer( buffer_, 255 ),
                           boost::bind( &SerialPort::read_callback,
                                        this,
                                        placeholders::error,
                                        placeholders::bytes_transferred ) );
    timeout_.expires_from_now( boost::posix_time::milliseconds( timeoutMs_ ) );
    timeout_.async_wait( boost::bind( &SerialPort::wait_callback, this, placeholders::error ) );
    dataAvailable_ = true;
}

void SerialPort::wait_callback( const boost::system::error_code& error ) {
    if( error ) {
        return;
    }
    port_.cancel();
}
