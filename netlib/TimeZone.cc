#include "./TimeZone.h"
#include "./Date.h"
#include "./noncopyable.h"

#include <vector>
#include <string>
#include <algorithm>

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <assert.h>

namespace netlib
{

struct transition
{
    time_t gmttime;
    time_t localtime;
    int localtimeIdx;

    transition(time_t t, time_t l, int localIdx)
        :   gmttime(t), localtime(l), localtimeIdx(localIdx)
    {}
};

struct Comp
{
    bool compareGmt;

    Comp(bool gmt) : compareGmt(gmt) {}

    bool operator()(const transition &lhs, const transition &rhs) const {
        if(compareGmt) {
            return lhs.gmttime < rhs.gmttime;
        }
        else {
            return lhs.localtime < rhs.localtime;
        }
    }

    bool equal(const transition &lhs, const transition &rhs) const {
        if(compareGmt) {
            return lhs.gmttime == rhs.gmttime;
        }
        else {
            return lhs.localtime == rhs.localtime;
        }
    }
};

struct LocalTime
{
    time_t gmtOffset;
    bool isDst;
    int arrbIdx;

    LocalTime(time_t offset, bool dst, int arrb)
     : gmtOffset(offset), isDst(dst), arrbIdx(arrb)
    {}
};

const int cSecondsPerDay = 24 * 60 * 60;

inline void fillHMS(int seconds, struct tm *t) {
    t->tm_sec = seconds % 60;
    int minutes = seconds / 60;
    t->tm_min = minutes % 60;
    t->tm_hour = minutes / 60;
}

class File
{
public:
    File(const char *zonefile)
        : _fp(::fopen(zonefile, "rb"))
    {
    }

    ~File() {
        if(_fp) {
            ::fclose(_fp);
        }
    }

    bool vaild() const {
        return _fp;
    }



private:
    FILE *_fp;
};

bool readTimeZoneFile(const char *zonefile, struct TimeZone::Data *data) {
    //
    File f(zonefile);

    if(f.vaild()) {

    }
}

const LocalTime* findLocalTime(const TimeZone::Data &data, transition sentry, Comp comp) {
    const LocalTime *local = NULL;

    if(data.transitions.empty() || comp(sentry, data.transitions.front())) {
        local = &data.localtimes.front();
    }
    else {
        std::vector<transition>::const_iterator transI = std::lower_bound(data.transitions.begin(),
                                                                          data.transitions.end(),
                                                                          sentry,
                                                                          comp);
        if(transI != data.transitions.end()) {
            if(!comp(sentry, *transI)) {
                assert(transI != data.transitions.begin());
                --transI;
            }
            local = &data.localtimes[transI->localtimeIdx];
        }
        else {
            local = &data.localtimes[data.transitions.back().localtimeIdx];
        }
    }

    return local;
}

}

using namespace netlib;

struct TimeZone::Data {
    std::vector<transition> transitions;
    std::vector<LocalTime> localtimes;
    std::vector<std::string> name;
    std::string abbrviation;
};

/*
TimeZone::TimeZone(const char *zonefile)
    : _data(new TimeZone::Data)
{
}
*/

TimeZone::TimeZone(int eastOfUtc, const char *tzname) 
    : _data(new TimeZone::Data)
{
    _data->localtimes.push_back(netlib::LocalTime(eastOfUtc, false, 0));
    _data->abbrviation = tzname;
}

struct tm TimeZone::toLocalTime(time_t seconds) const {
    /// UTC时间转化为本地时间
    /// 转入的参数为UTC时间

    struct tm localTime;
    ::bzero(&localTime, sizeof localTime);
    assert(_data != NULL);
    const Data &data(*_data);

    netlib::transition senry(seconds, 0, 0);
    const netlib::LocalTime *local = netlib::findLocalTime(data, senry, netlib::Comp(true));

    if(local) {
        time_t localSeconds = seconds + local->gmtOffset;
        ::gmtime_r(&localSeconds, &localTime);
        localTime.tm_isdst = local->isDst;
        localTime.tm_gmtoff = local->gmtOffset;
        localTime.tm_zone = &data.abbrviation[local->arrbIdx];
    }
    return localTime;
}

time_t TimeZone::fromLocalTime(const struct tm &localtime) const {
    /// 将本地时间转化为1970年1月1日以来的秒数
    assert(_data != NULL);
    const Data &data(*_data);

    struct tm tmp = localtime;
    time_t seconds = ::timegm(&tmp);
    netlib::transition sentry(0, seconds, 0);
    const netlib::LocalTime *local = netlib::findLocalTime(data, sentry, Comp(false));

    if(localtime.tm_isdst) {
        /// 夏令时
        struct tm tryTm = toLocalTime(seconds - local->gmtOffset);
        if(!tryTm.tm_isdst
            && tryTm.tm_hour == localtime.tm_hour
            && tryTm.tm_min == localtime.tm_min)
        {
            seconds -= 3600;
        }
    }

    return seconds - local->gmtOffset;
}

struct tm TimeZone::toUtcTime(time_t secondsSinceEpoch, bool yday) {
    /// 转化为UTC时间
    struct tm utc;
    ::bzero(&utc, sizeof utc);
    utc.tm_zone = "GMT";
    int seconds = static_cast<int>(seconds % cSecondsPerDay);
    int days = static_cast<int>(seconds / cSecondsPerDay);

    if(seconds < 0) {
        seconds += cSecondsPerDay;
        --days;
    }
    netlib::fillHMS(seconds, &utc);
    Date date(days+Date::cJulianDayOf1970_01_01);
    Date::YearMonthDay ymd = date.yearMonthDay();

    utc.tm_year = ymd.year - 1900;
    utc.tm_mon = ymd.month - 1;
    utc.tm_mday = ymd.day;
    utc.tm_wday = date.weekDay();

    if(yday) {
        Date startOfYear(ymd.year, 1, 1);
        utc.tm_yday = date.julianDayNumber() - startOfYear.julianDayNumber();
    }

    return utc;
}

time_t TimeZone::fromUtcTime(const struct tm &utc) {
    return fromUtcTime(utc.tm_year+1900, utc.tm_mon+1, utc.tm_mday,
                         utc.tm_hour, utc.tm_min, utc.tm_sec);
}

time_t TimeZone::fromUtcTime(int year, int month, int days, int hour, int minutes, int seconds) {
    Date date(year, month, days);
    int secondsInDay = hour * 3600 + minutes * 60 + seconds;
    time_t day = date.julianDayNumber() - Date::cJulianDayOf1970_01_01;
    return day * netlib::cSecondsPerDay + secondsInDay;
}


