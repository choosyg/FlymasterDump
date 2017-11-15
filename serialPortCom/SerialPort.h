#pragma once

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

enum class Speed : unsigned int { BAUD9600 = 9600, BAUD57600 = 57600, BAUD115200 = 115200 };

class SerialPort {
public:
    SerialPort( const std::string& port );
    ~SerialPort();

    void setSpeed( Speed s );
    void setTimeout( unsigned int ms );

    std::string request( const std::string& request );

private:
    boost::asio::io_service ioService_;
    boost::asio::serial_port port_;

    unsigned int timeoutMs_;
    boost::asio::deadline_timer timeout_;
    char buffer_[256];
    bool dataAvailable_;
    std::string response_;

    void writeRequest( const std::string& request );
    std::string readResponse();
    void read_callback( const boost::system::error_code& error, std::size_t bytes_transferred );
    void wait_callback( const boost::system::error_code& error );
};
