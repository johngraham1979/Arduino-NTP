//
// @Author John Graham
// @Date 15/02/2018.
//

#ifndef NTP_H
#define NTP_H

#include "IPAddress.h"
#include "WiFiUdp.h"
#include <vector>

using namespace std;

class WiFiUDP;
class UDP;

/*
 * Performs communication with NTP time servers.
 *
 * Maintains a collection of NTP server IP addresses,
 * constructs and sends NTP request,
 * and decodes the time from the response.
 */
class NTP
{

private:
    // Collection of NTP server IP addresses.
    vector<IPAddress*> m_IPAddresses;

    UDP* m_udp = nullptr;

    // Message buffer
    byte* m_packetBuffer = nullptr;
    int m_NtpPacketSize = 48;

    // Constant for helping deducing the time from the response.
    const unsigned long SEVENTY_YEARS = 2208988800UL;

    /*
     * Constructs NTP request.
     *
     * Zeroes out message buffer,
     * and sets values for NTP request.
     */
    void constructRequest()
    {
        // Zero out message buffer.
        memset( m_packetBuffer, 0, m_NtpPacketSize );

        m_packetBuffer[ 0 ] = 0b11100011;      // LI, Version, Mode
        m_packetBuffer[ 1 ] = 0;               // Stratum, or type of clock
        m_packetBuffer[ 2 ] = 6;               // Polling Interval
        m_packetBuffer[ 3 ] = 0xEC;            // Peer Clock Precision

        // 8 bytes of zero for Root Delay & Root Dispersion
        m_packetBuffer[ 12 ] = 49;
        m_packetBuffer[ 13 ] = 0x4E;
        m_packetBuffer[ 14 ] = 49;
        m_packetBuffer[ 15 ] = 52;
    }

    /*
     * Disable default constructor.
     */
    NTP(){}
public:

    /*
     * Constructor.
     *
     * @param t_udp, UDP object for performing UDP communication.
     * WiFiUDP derives from UDP and can be used when this
     * communication is performed via WiFi.
     */
    NTP( UDP* t_udp )
    {
        m_udp = t_udp;
        m_packetBuffer = new byte[m_NtpPacketSize];
    }

    /*
     * Destructor.
     */
    ~NTP()
    {
        delete (m_packetBuffer);
        delete (m_udp);

        for( auto iter = m_IPAddresses.begin(); iter != m_IPAddresses.end(); iter++ )
            delete (*iter);
    }

    /*
     * Adds IP address to collection of NTP server IP addresses.
     *
     * @param t_ipAddress, NTP server IP address.
     */
    void addServerIP( IPAddress* t_ipAddress )
    {
        m_IPAddresses.push_back( t_ipAddress );
    }

    /*
     * Initialize UDP object for communication.
     *
     * @param t_port, Local port number.
     */
    void begin( const unsigned int t_port )
    {
        m_udp->begin( t_port );
    }

    /*
     * Getter for UDP object.
     */
    const UDP* getUDP()
    {
        return m_udp;
    }

    /*
     * Checks response.
     *
     * @return, 0 for failed response or the size of the response.
     */
    int parsePacket()
    {
        return m_udp->parsePacket();
    }

    /*
     * Conducts outbound NTP/UDP communication.
     *
     * Iterates over collection of NTP server IP addresses until a response is
     * successfully received within 2 seconds.
     *
     * @return, True if a response is successfully received, false otherwise.
     */
    bool sendRequest()
    {
        std::vector<IPAddress*>::iterator iter, end;
        IPAddress* ipAddress;
        // Iterate over NTP IP addresses until a successful response.
        for( auto iter = m_IPAddresses.begin(); iter != m_IPAddresses.end(); iter++ )
        {
            constructRequest();

            ipAddress = *iter;

            Serial.print( "Sending NTP request to IP address " );
            Serial.print( ipAddress->toString());

            m_udp->beginPacket( *ipAddress, 123 ); //NTP requests are to port 123
            m_udp->write( m_packetBuffer, m_NtpPacketSize );
            m_udp->endPacket();

            delay(2000);

            if( parsePacket())
            {
                Serial.println( ": succeeded." );
                return true;
            }

            Serial.println( ": failed." );
        }
        return false;
    }

    /*
     * Extracts the time from NTP response.
     *
     * @return, Time in seconds as delivered in response.
     */
    unsigned long decodeSeconds()
    {
        m_udp->read( m_packetBuffer, m_NtpPacketSize );

        const unsigned long highWord = word( m_packetBuffer[ 40 ], m_packetBuffer[ 41 ] );
        const unsigned long lowWord = word( m_packetBuffer[ 42 ], m_packetBuffer[ 43 ] );

        const unsigned long secsSince1900 = highWord << 16 | lowWord;

        return secsSince1900 - SEVENTY_YEARS;
    }
};

#endif //NTP_H
