#include <stdio.h>
#include <inttypes.h> // PRIi64, PRIi32
#include <time.h>

#include "tai.h"

int main()
{
    tai_t now = tai_now();
    printf("Current time:                    %"PRIi64"\n", now);

    tain_t now_ns = tain_now();
    printf("Current time (with nanoseconds): %"PRIi64".%"PRIi32"\n", now_ns.sec, now_ns.ns);

    printf("\n");

    printf("Accounting for leap seconds:\n");

    struct tm tm;
    printf("2015-06-30T23:59:59Z\n");
    strptime("2015-06-30T23:59:59Z", "%Y-%m-%dT%H:%M:%SZ", &tm);
    printf("TAI:  %"PRIi64"\n", tai_mktime(&tm));
    printf("UNIX: %ld\n", mktime(&tm));

    printf("2015-06-30T23:59:60Z\n");
    strptime("2015-06-30T23:59:60Z", "%Y-%m-%dT%H:%M:%SZ", &tm);
    printf("TAI:  %"PRIi64"\n", tai_mktime(&tm));
    printf("UNIX: %ld <== REPEATS\n", mktime(&tm));

    printf("2015-07-01T00:00:00Z\n");
    strptime("2015-07-01T00:00:00Z", "%Y-%m-%dT%H:%M:%SZ", &tm);
    printf("TAI:  %"PRIi64"\n", tai_mktime(&tm));
    printf("UNIX: %ld\n", mktime(&tm));

    printf("\n");
    printf("Date with nanoseconds:\n");
    printf("2015-07-02T12:34:56,28931029Z\n");

    strptime("2015-07-02T12:34:56Z", "%Y-%m-%dT%H:%M:%SZ", &tm);
    tain_t time = tain_mktime(&tm, 28931029);

    printf("TAI:  %"PRIi64".%"PRIi32"\n", time.sec, time.ns);

    return 0;
}
