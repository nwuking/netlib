#include "netlib/base/TimeZone.h"

#include <iostream>

#include <string.h>

struct tm getTm(int year, int month, int day,
                int hour, int minute, int second)
{
    struct tm gmt;
    ::bzero(&gmt, sizeof(gmt));
    gmt.tm_year = year - 1900;
    gmt.tm_mon = month - 1;
    gmt.tm_yday = day;
    gmt.tm_hour = hour;
    gmt.tm_min = minute;
    gmt.tm_sec = second;
    return gmt;
}

struct tm getTm(const char *str) {
    struct tm gmt;
    ::bzero(&gmt, sizeof(gmt));
    return gmt;
}

int main() {
    return 0;
}