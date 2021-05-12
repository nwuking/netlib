#include "./Date.h"

#include <stdio.h>
#include <time.h>

namespace netlib
{

char require_32_bit_integer_at_least[sizeof(int) >= sizeof(int32_t) ? 1 : -1];

int getJulianDayNumber(int year, int month, int day) {
    (void)require_32_bit_integer_at_least;
    int a = (14-month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day+(153*m+2)/5+y*365+y/4-y/100+y/400-32045; 
}

struct Date::YearMonthDay getYearMonthDay(int julianNumber) {
    int a = julianNumber + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - ((b*146097) / 4);
    int d = (4*c+3)/1461;
    int e = c - ((d*1461)/4);
    int m = (5*e+2)/153;
    Date::YearMonthDay ymd;
    ymd.day = e - ((153*m+2)/5)+1;
    ymd.month = m+3-12*(m/10);
    ymd.year = b*100+d - 4800 + (m/10);
    return ymd;
}

const int Date::cJulianDayOf1970_01_01 = getJulianDayNumber(1970, 1, 1);

}

using namespace netlib;

Date::Date(int y, int m, int d)
    : _julianDayNumber(netlib::getJulianDayNumber(y,m,d))
{
}

Date::Date(const struct tm &t)
 : _julianDayNumber(netlib::getJulianDayNumber(t.tm_year+1900,
                                                t.tm_mon+1,
                                                t.tm_mday))
{
}

Date::YearMonthDay Date::yearMonthDay() const {
    return netlib::getYearMonthDay(_julianDayNumber);
}

