#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import requests
import datetime

r = requests.get('https://www.ietf.org/timezones/data/leap-seconds.list')

times = [line for line in r.content.split('\n')[:-1] if line[0] != '#']
times = [line.split('\t')[:2] for line in times]

# Convert from NTP timestamp to UNIX timestamp
def convert(time):
    NTP2UNIX = (70 * 365 + 17) * 86400
    time = int(time)
    t = time - NTP2UNIX;
    while t < 0:
        t += 65536 * 65536
    return t

# ============= leapsecsat_unix =============

print """unsigned int leapsecsat_unix(time_t date)
{
    #define TABLE_LEN """ + str(len(times)) + """
    time_t leapsec_dates[TABLE_LEN] = { 0 };
    uint8_t leapsec_diff[TABLE_LEN] = { 0 };

    #define new_leapsec(i, date, diff) leapsec_dates[i] = date; leapsec_diff[i] = diff;"""
for i, leapsec in enumerate(times):
    print '    new_leapsec(%d, %d, %s);' % (i, convert(leapsec[0]), leapsec[1])

print """    #undef new_leapsec

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
}"""

# ============= leapsecsat_tai =============
print

print """unsigned int leapsecsat_tai(tai_t date)
{
    #define TABLE_LEN """ + str(len(times)) + """
    time_t leapsec_dates[TABLE_LEN] = { 0 };
    uint8_t leapsec_diff[TABLE_LEN] = { 0 };

    #define new_leapsec(i, date, diff) leapsec_dates[i] = date; leapsec_diff[i] = diff;"""
for i, leapsec in enumerate(times):
    print '    new_leapsec(%d, %d, %s);' % (i, convert(leapsec[0]) + int(leapsec[1]), leapsec[1])

print """    #undef new_leapsec

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
}"""

# ============= isleapsec_tai =============
print

print """bool isleapsec_tai(tai_t date)
{
    #define TABLE_LEN """ + str(len(times)) + """
    time_t leapsec_dates[TABLE_LEN] = { 0 };
    uint8_t leapsec_diff[TABLE_LEN] = { 0 };

    #define new_leapsec(i, date, diff) leapsec_dates[i] = date; leapsec_diff[i] = diff;"""
for i, leapsec in enumerate(times):
    print '    new_leapsec(%d, %d, %s);' % (i, convert(leapsec[0]) + int(leapsec[1]) - 1, leapsec[1])

print """    #undef new_leapsec

    // go backwards since it's more likely that people will be using modern dates
    for (int i = TABLE_LEN - 1; i >= 0; i--)
    {
        if (leapsec_dates[i] == date)
        {
            return true;
        }
    }
    #undef TABLE_LEN
    // not a leap second
    return false;
}"""
