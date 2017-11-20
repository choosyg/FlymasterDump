#include "Request.h"

#include "ascii/StringUtils.h"
#include "binary/Convert.h"

namespace {

enum class Status : uint8_t { Ok = 0xb1, Error = 0xb2, Cancel = 0xb3 };

std::string toString( Status s ) {
    return toBytes( static_cast< uint8_t >( s ) );
}

} // namespace

std::string requestSerial( SerialPort& port, Callback& cb ) {
    cb.notify( Severity::Debug, "Requesting serial" );
    auto serial = port.request( "$PFMSNP,*3A" );
    cb.notify( Severity::Debug, "Serial: " + serial );
    return serial;
}

std::vector< FlightMetaInfo > requestFlightInfos( SerialPort& port, Callback& cb ) {
    cb.notify( Severity::Info, "Requesting flight infos" );
    auto list = split( port.request( "$PFMDNL,LST,*56" ), '\n' );
    std::vector< FlightMetaInfo > fmi;
    for( const auto& item : list ) {
        cb.notify( Severity::Debug, "Received: " + item );
        if( item.empty() ) { // The last item is empty because of the split
            continue;
        }
        NmeaSentence sentence( item );
        fmi.emplace_back( sentence );
    }
    cb.notify( Severity::Info, "#Flights: " + std::to_string( fmi.size() ) );
    return fmi;
}

BinaryFlightData requestFlight( SerialPort& port, FlightMetaInfo& info, Callback& cb ) {
    cb.notify( Severity::Info, "Requesting Flight #" + std::to_string( info.index ) );

    std::vector< BinaryBlock > blocks;
    size_t errorCount = 0;
    size_t bytes = 0;
    std::string request = info.flightRequest();
    while( true ) {
        try {
            BinaryBlock block = port.request( request );
            if( block.packetId == PacketId::END_OF_DATA ) {
                break;
            }
            blocks.push_back( block );
            request = toString( Status::Ok );
            errorCount = 0;
            cb.notify( Severity::Debug, "OK" );
            if( cb.isCancelled() ) {
                port.request( toString( Status::Cancel ) );
                break;
            }
            bytes += block.data.size();
            cb.notify( Severity::Progress, "Received "+std::to_string(bytes)+" bytes" );
        } catch( std::exception& e ) {
            ++errorCount;
            if( errorCount == 3 ) {
                cb.notify( Severity::Critical, std::string( "Aborting: " ) + e.what() );
                port.request( toString( Status::Cancel ) );
                break;
            } else {
                cb.notify( Severity::Warning, std::string( "Retry: Received invalid block: " ) + e.what() );
                request = toString( Status::Error );
            }
        }
    }
    cb.notify( Severity::Info, "#Blocks: " + std::to_string( blocks.size() ) );
    return blocks;
}
