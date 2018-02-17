//
// @Author John Graham
// @Date 15/02/2018.
//

#ifndef TIME_CONVERTER_H
#define TIME_CONVERTER_H

#include <String>
#include <stdlib.h>

/*
 * Singleton utility for deducing the hours, minutes and seconds from
 * the time represented in total seconds. Also supplies the time
 * in the form of a c-style string.
 */
class TimeConverter
{
private:
    // UTC offset. This is set for the desired time zone.
    unsigned short m_utcOffset = 0;
    // Single instance of this utility class.
    static TimeConverter* m_timeConverter;
    // C-style string for storing the time in the format HH:MM:SS.
    char timeString[8] = { '\0' };

    /*
     * Private default constructor.
     */
    TimeConverter()
    {

    }

    /*
     * Private constructor for also setting the UTC offset.
     */
    TimeConverter( const unsigned short t_utcOffset )
    {
        m_utcOffset = t_utcOffset;
    }

    /*
     * Helper function intended for coping the hours, minutes or seconds
     * into a particular position in a c-style string.
     * If the number is only one digit then prepend a leading '0' so that
     * the HH, MM or SS is always 2 characters.
     *
     * @param t_timeString, The destination array.
     * @param t_number, The hours, minutes or seconds.
     */
    void copyTime( char* t_timeString, unsigned short t_number )
    {
        if( t_number < 10 )
        {
            t_timeString[ 0 ] = '0';
            sprintf( &t_timeString[ 1 ], "%d", t_number );
        } else
            sprintf( &t_timeString[ 0 ], "%d", t_number );
    }

public:
    ~TimeConverter()
    {
        delete (m_timeConverter);
    }

    /*
     * TimeConverter::getInstance() returns the single instance
     * of this time-utility class.
     *
     * @return, The single instance of this time-utility class.
     */
    static TimeConverter* getInstance()
    {
        if( m_timeConverter == nullptr )
            m_timeConverter = new TimeConverter();
        return m_timeConverter;
    }

    /*
     * TimeConverter::getInstance(const unsigned short)
     * returns the single instance of this time-utility class,
     * while setting the UTC offset for a particular time zone.
     *
     * The offset will be taken into account when deducing the hours.
     *
     * @param t_offset, UTC offset for a particular time zone.
     * @return, The single instance of this time-utility class.
     */
    static TimeConverter* getInstance( const unsigned short t_utcOffset )
    {
        if( m_timeConverter == nullptr )
            m_timeConverter = new TimeConverter( t_utcOffset );
        return m_timeConverter;
    }

    /*
     * Deduce the hours as it would appear in the format HH:MM:SS.
     *
     * @param t_timeInSeconds, The time expressed in seconds.
     * @return, The number of full hours expressed by t_timeInSeconds, base 24.
     */
    unsigned short getHours( const unsigned long t_timeInSeconds )
    {
        return ( short ) ((((t_timeInSeconds / 60) / 60) + m_utcOffset) % 24);
    }

    /*
     * Deduce the minutes as it would appear in the format HH:MM:SS.
     *
     * @param t_timeInSeconds, The time expressed in seconds.
     * @return, The number of full minutes expressed by t_timeInSeconds, base 60.
     */
    unsigned short getMinutes( const unsigned long t_timeInSeconds )
    {
        return ( short ) ((t_timeInSeconds / 60) % 60);
    }

    /*
     * Deduce the seconds as it would appear in the format HH:MM:SS.
     *
     * @param t_timeInSeconds, The time expressed in seconds.
     * @return, The number of seconds expressed by t_timeInSeconds, base 60.
     */
    unsigned short getSeconds( const unsigned long t_timeInSeconds )
    {
        return ( short ) (t_timeInSeconds % 60);
    }

    /*
     * Construct and return a c-style string expressing the time
     * in the format HH:MM:SS.
     *
     * @param t_timeInSeconds, The time expressed in seconds.
     * @return, c-style string expressing the time in the
     * format HH:MM:SS.
     */
    const char* getTimeCString( const unsigned long t_seconds )
    {
        const unsigned short hours = getHours( t_seconds );
        copyTime( &timeString[ 0 ], hours );

        timeString[ 2 ] = ':';
        const unsigned short minutes = getMinutes( t_seconds );
        copyTime( &timeString[ 3 ], minutes );

        timeString[ 5 ] = ':';

        const unsigned short seconds = getSeconds( t_seconds );
        copyTime( &timeString[ 6 ], seconds );

        timeString[ 8 ] = '\0';
        return timeString;
    }

    /*
     * Gets the last received time in the format HH:MM:SS as a c-style string.
     *
     * @return, C-style string expressing the time in the format HH:MM:SS
     */
    const char* getPreviousTimeCString()
    {
        return timeString;
    }
};

// Initialize single (static) instance to 'nullptr' so its assignment can be checked.
TimeConverter* TimeConverter::m_timeConverter = nullptr;

#endif //TIME_CONVERTER_H
