/* (c) 2015 Alastair Feille
   MIT License */

#include "tai.h"

unsigned int leapsecsat_unix(time_t date)
{
    #define TABLE_LEN 27
    time_t leapsec_dates[TABLE_LEN] = { 0 };
    uint8_t leapsec_diff[TABLE_LEN] = { 0 };

    #define new_leapsec(i, date, diff) leapsec_dates[i] = date; leapsec_diff[i] = diff;
    new_leapsec(0, 63072000, 10);
    new_leapsec(1, 78796800, 11);
    new_leapsec(2, 94694400, 12);
    new_leapsec(3, 126230400, 13);
    new_leapsec(4, 157766400, 14);
    new_leapsec(5, 189302400, 15);
    new_leapsec(6, 220924800, 16);
    new_leapsec(7, 252460800, 17);
    new_leapsec(8, 283996800, 18);
    new_leapsec(9, 315532800, 19);
    new_leapsec(10, 362793600, 20);
    new_leapsec(11, 394329600, 21);
    new_leapsec(12, 425865600, 22);
    new_leapsec(13, 489024000, 23);
    new_leapsec(14, 567993600, 24);
    new_leapsec(15, 631152000, 25);
    new_leapsec(16, 662688000, 26);
    new_leapsec(17, 709948800, 27);
    new_leapsec(18, 741484800, 28);
    new_leapsec(19, 773020800, 29);
    new_leapsec(20, 820454400, 30);
    new_leapsec(21, 867715200, 31);
    new_leapsec(22, 915148800, 32);
    new_leapsec(23, 1136073600, 33);
    new_leapsec(24, 1230768000, 34);
    new_leapsec(25, 1341100800, 35);
    new_leapsec(26, 1435708800, 36);
    #undef new_leapsec

    // go backwards since it's more likely that people will be using modern dates
    for (int i = TABLE_LEN - 1; i >= 0; i--)
    {
        if (leapsec_dates[i] - date <= 0)
        {
            return leapsec_diff[i];
        }
    }
    #undef TABLE_LEN
    // some date before the first leap seconds were inserted
    return leapsec_diff[0];
}

unsigned int leapsecsat_tai(tai_t date)
{
    #define TABLE_LEN 27
    time_t leapsec_dates[TABLE_LEN] = { 0 };
    uint8_t leapsec_diff[TABLE_LEN] = { 0 };

    #define new_leapsec(i, date, diff) leapsec_dates[i] = date; leapsec_diff[i] = diff;
    new_leapsec(0, 63072010, 10);
    new_leapsec(1, 78796811, 11);
    new_leapsec(2, 94694412, 12);
    new_leapsec(3, 126230413, 13);
    new_leapsec(4, 157766414, 14);
    new_leapsec(5, 189302415, 15);
    new_leapsec(6, 220924816, 16);
    new_leapsec(7, 252460817, 17);
    new_leapsec(8, 283996818, 18);
    new_leapsec(9, 315532819, 19);
    new_leapsec(10, 362793620, 20);
    new_leapsec(11, 394329621, 21);
    new_leapsec(12, 425865622, 22);
    new_leapsec(13, 489024023, 23);
    new_leapsec(14, 567993624, 24);
    new_leapsec(15, 631152025, 25);
    new_leapsec(16, 662688026, 26);
    new_leapsec(17, 709948827, 27);
    new_leapsec(18, 741484828, 28);
    new_leapsec(19, 773020829, 29);
    new_leapsec(20, 820454430, 30);
    new_leapsec(21, 867715231, 31);
    new_leapsec(22, 915148832, 32);
    new_leapsec(23, 1136073633, 33);
    new_leapsec(24, 1230768034, 34);
    new_leapsec(25, 1341100835, 35);
    new_leapsec(26, 1435708836, 36);
    #undef new_leapsec

    // go backwards since it's more likely that people will be using modern dates
    for (int i = TABLE_LEN - 1; i >= 0; i--)
    {
        if (leapsec_dates[i] - date <= 0)
        {
            return leapsec_diff[i];
        }
    }
    #undef TABLE_LEN
    // some date before the first leap seconds were inserted
    return leapsec_diff[0];
}

tai_t tai_now()
{
    time_t now = time(NULL);
    return now + leapsecsat_unix(now);
}

tain_t tain_now()
{
    tain_t now;

#ifdef _WIN32
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t tt = ft.dwHighDateTime;
    tt <<= 32;
    tt |= ft.dwLowDateTime;
    tt /= 10;
    tt -= 11644473600000000ULL;
    now.sec = tt / 1000000;
    now.ns = tt % 1000000 * 1000;
#else
    struct timespec tt;
    clock_gettime(CLOCK_REALTIME, &tt);
    now.sec = tt.tv_sec;
    now.ns = tt.tv_nsec;
#endif

    // factor in leap seconds
    now.sec += leapsecsat_unix(now.sec);

    return now;
}

tai_t tai_mktime(struct tm* date)
{
    // if leap second
    if (date->tm_hour == 23 && date->tm_min == 59 && date->tm_sec == 60)
    {
        /*
        #	Many systems of this type effectively stop the system clock for
        #	one second during the leap second and use a time that is equivalent
        #	to 23:59:59 UTC twice. For these systems, the corresponding TAI
        #	timestamp would be obtained by advancing to the next entry in the
        #	following table when the time equivalent to 23:59:59 UTC
        #	is used for the second time. Thus the leap second which
        #	occurred on 30 June 1972 at 23:59:59 UTC would have TAI
        #	timestamps computed as follows:
        #
        #	...
        #	30 June 1972 23:59:59 (2287785599, first time):	TAI= UTC + 10 seconds
        #	30 June 1972 23:59:60 (2287785599,second time):	TAI= UTC + 11 seconds
        #	1  July 1972 00:00:00 (2287785600)		TAI= UTC + 11 seconds
        #	...
        #
        #	If your system realizes the leap second by repeating 00:00:00 UTC twice
        #	(this is possible but not usual), then the advance to the next entry
        #	in the table must occur the second time that a time equivalent to
        #	00:00:00 UTC is used. Thus, using the same example as above:
        #
        #	...
        #       30 June 1972 23:59:59 (2287785599):		TAI= UTC + 10 seconds
        #       30 June 1972 23:59:60 (2287785600, first time):	TAI= UTC + 10 seconds
        #       1  July 1972 00:00:00 (2287785600,second time):	TAI= UTC + 11 seconds
        #	...
        (from https://www.ietf.org/timezones/data/leap-seconds.list)
        */

        // use the second before so that you end up with the same base time
        date->tm_sec--;
        time_t base_time = timegm(date);
        // set date back to actual date
        date->tm_sec++;
        // then get the leap second at 23:59:60 by asking for leapsecs at 00:00:00
        uint8_t leapsecs = leapsecsat_unix(base_time + 1);

        return base_time + leapsecs;
    }

    // if not a leap second
    time_t unix_time = timegm(date);
    return unix_time + leapsecsat_unix(unix_time);
}

struct tm* tai_utctime(tai_t time)
{
    struct tm* utc = gmtime(&time);
    utc->tm_sec -= leapsecsat_tai(time);
    // normalize
    mktime(utc);

    return utc;
}

tain_t tain_mktime(struct tm* date, uint32_t ns)
{
    tain_t ret;
    ret.sec = tai_mktime(date);
    ret.ns = ns;

    return ret;
}

uint64_t tai_tobytes(tai_t time)
{
    return htonll(time);
}

tai_t tai_frombytes(uint64_t time)
{
    return ntohll(time);
}

tain_t tain_tobytes(tain_t time)
{
    tain_t ret;
    ret.sec = htonll(time.sec);
    ret.sec = htonl(time.ns);

    return ret;
}

tain_t tain_frombytes(tain_t time)
{
    tain_t ret;
    ret.sec = ntohll(time.sec);
    ret.sec = ntohl(time.ns);

    return ret;
}
