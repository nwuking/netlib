#include "netlib/base/TimeZone.h"

#include <iostream>

#include <string.h>
#include <time.h>

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
    ::strptime(str, "%F %T", &gmt);
    return gmt;
}

time_t getGmt(int year, int month, int day,
              int hour, int minute, int seconds)
{
    struct tm gmt = getTm(year, month, day, hour, minute, seconds);
    return timegm(&gmt);
}

time_t getGmt(const char *str) {
    struct tm gmt = getTm(str);
    return timegm(&gmt);
}

struct testCase {
    const char *gmt;
    const char *local;
    bool isdist;
};

void test(const netlib::TimeZone &tz, testCase tc) {
    time_t gmt = getGmt(tc.gmt);        /// utc
    {
    struct tm local = tz.toLocalTime(gmt);
    char buf[256];
    ::strftime(buf, sizeof buf, "%F %T%z(%Z)", &local);
    if(::strcmp(buf, tc.local) != 0 || tc.isdist != local.tm_isdst) {
        std::cout << "WRONG: ";
    }
    std::cout << tc.gmt << " -> " << buf << "\n";
    //std::cout << tc.isdist << " : " << local.tm_isdst << "\n";
    }

    {
        struct tm local = getTm(tc.local);
        local.tm_isdst = tc.isdist;
        time_t result = tz.fromLocalTime(local);
        if(result != gmt) {
            struct tm local2 = tz.toLocalTime(result);
            char buf[256];
            strftime(buf, sizeof buf, "%F %T%z(%Z)", &local2);
            std::cout << "WRONG fromLocalTime " << static_cast<long>(gmt)
                      << " " << static_cast<long>(result) << " " << buf << "\n";
        }
    }
}

void testFixedTimeZone() {
    netlib::TimeZone tz(8*3600, "CST");
    testCase cases[] = {
        {"2021-06-03 00:00:00", "2021-06-03 08:00:00+0800(CST)", false},
    };

    for(const testCase &t : cases) {
        test(tz, t);
    }
}

void testUtc() {
    int kRange = 100 * 1000 * 1000;
    for(time_t t = 10; t <= kRange; t+=11) {
        struct tm *t1 = ::gmtime(&t);
        struct tm t2 = netlib::TimeZone::toUtcTime(t, true);
        char buf[80], buf2[80];
        strftime(buf, sizeof buf, "%F %T %u %j", t1);
        strftime(buf2, sizeof buf2, "%F %T %u %j", &t2);
        if(::strcmp(buf, buf2) != 0) {
            std::cout << buf << " != " << buf2 << "\n";
            exit(-1);
        }
        time_t t3 = netlib::TimeZone::fromUtcTime(t2);
        if(t != t3) {
            std::cout << t << " != " << t3 << "\n";
            exit(-1);
        }
    }
}
#if 1
void testBeijing() {
    netlib::TimeZone tz("/usr/share/zoneinfo/Asia/Beijing");
    testCase tests[] = {
        {"2021-06-04 00:00:00", "2021-06-04 08:00:00+0800(CST)", false},
    };

    for(const auto &c : tests) {
        test(tz, c);
    }
}
#endif

int main() {
    testFixedTimeZone();
    #if 0
    testUtc();
    #endif
    testBeijing();
    return 0;
}