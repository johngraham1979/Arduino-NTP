#include <TimeConverter.h>
#include <NTP.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


const char* SSID = "Your New Wi-Fi";
const char* SSIDPWD = "170420502251";

const unsigned short LOCAL_PORT = 2390;
const unsigned short UTC_OFFSET = 13;


// This code example conducts NTP communication via WiFi.
// We can write 'new UDP()' in place of 'new WiFiUDP()' for communication via RJ45.
NTP ntp( new WiFiUDP() );

/*
 * Some initial setting-up.
 */
void setup()
{
  // Add any number of NTP server IP addresses.
  ntp.addServerIP( new IPAddress( 122, 252, 188, 99 ) );
  ntp.addServerIP( new IPAddress( 103, 242, 68, 68 ) );
  ntp.addServerIP( new IPAddress( 203, 118, 151, 32 ) );

  // Start WiFi connection.
  WiFi.mode( WIFI_STA );
  WiFi.begin( SSID, SSIDPWD );

  // Allow the printing of information to the Serial output window.
  Serial.begin( 115200 );

  // Animate the establishing of the WiFi connection to the Serial output window.
  Serial.print( "Connecting" );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay( 500 );
    Serial.print( "." );
  }
  Serial.println();

  // Start NTP's UDP functionality.
  ntp.begin( LOCAL_PORT );
}


/*
 * Constant looping.
 * Send request, decode time-in-seconds from response, get time as a string and print to Serial output window.
 */
void loop()
{
  // send NTP request, and check success...
  if ( ntp.sendRequest() ) 
  {
    // If successful, decode the seconds, and get the time.
    const char* timeString = TimeConverter::getInstance( UTC_OFFSET )->getTimeCString( ntp.decodeSeconds() );
    
    // Do something with the time string, i.e. print it to the Serial output window.
    Serial.println( timeString );
    
    Serial.println( "(Next attempt in 10 seconds)" );
    delay( 8000 );
  }
  else
  {
    // Failed NTP request.
    Serial.println( "(Next attempt in 4 seconds)" );
    delay( 2000 );
  }
  
  delay( 2000 );
}
