#include "netlib/base/TimeZone.h"
#include "netlib/base/Date.h"
#include "netlib/base/noncopyable.h"

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

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

struct TimeZone::Data {
    std::vector<transition> transitions;
    std::vector<LocalTime> localtimes;
    std::vector<std::string> name;
    std::string abbrviation;
};

#if 0
class File : NonCopyAble
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

    std::string readBytes(int n) {
        char buf[n];
        ssize_t nr = ::fread(buf, 1, n, _fp);
        if(nr != n) {
            throw std::logic_error("no enough data");
        }
        return std::string(buf, n);
    }

    int32_t readInt32() {
        int32_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(int32_t), _fp);
        if(nr != sizeof(int32_t)) {
            throw std::logic_error("bad int32_t data");
        }
        return be32toh(x);
    }

    uint8_t readUint8() {
        uint8_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), _fp);
        if(nr != sizeof(uint8_t)) {
            throw std::logic_error("bad uint8_t data");
        }
        return x;
    }

private:
    FILE *_fp;
};

bool readTimeZoneFile(const char *zonefile, struct TimeZone::Data *data) {
    File f(zonefile);
    if(f.vaild()) {
        try {
            std::string head = f.readBytes(4);
            if(head != "TZif")
                throw std::logic_error("bad head");
            std::string version = f.readBytes(1);
            f.readBytes(15);
            int32_t isgmtcnt = f.readInt32();
            int32_t isstdcnt = f.readInt32();
            int32_t leapcnt = f.readInt32();
            int32_t timecnt = f.readInt32();
            int32_t typecnt = f.readInt32();
            int32_t charcnt = f.readInt32();
            std::vector<int32_t> trans;
            std::vector<int> localtimes;
            trans.reserve(timecnt);
            for(int i = 0; i < timecnt; ++i) {
                trans.push_back(f.readInt32());
            }
            for(int i = 0; i < timecnt; ++i) {
                uint8_t local = f.readUint8();
                localtimes.push_back(local);
            }
            for(int i = 0; i < typecnt; ++i) {
                int32_t gmtoff = f.readInt32();
                uint8_t isdist = f.readUint8();
                uint8_t abbrind = f.readUint8();

                data->localtimes.push_back(LocalTime(gmtoff, isdist, abbrind));
            }
            for(int i = 0; i < timecnt; ++i) {
                int localIndx = localtimes[i];
                time_t lotime = trans[i] + data->localtimes[localIndx].gmtOffset;
                data->transitions.push_back(transition(trans[i], lotime, localIndx));
            }

            data->abbrviation = f.readBytes(charcnt);
            for(int i = 0; i < leapcnt; ++i) {
                //
            }
            (void)isgmtcnt;
            (void)isstdcnt;
        }
        catch(std::logic_error &e) {
            fprintf(stderr, "%s\n", e.what());
        }
    }
    return true;
}
#endif

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

#if 0
TimeZone::TimeZone(const char *zonefile)
    : _data(new TimeZone::Data)
{
    if(!netlib::readTimeZoneFile(zonefile, _data.get())) {
        _data.reset();
    }
}
#endif

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
    int seconds = static_cast<int>(secondsSinceEpoch % cSecondsPerDay);
    int days = static_cast<int>(secondsSinceEpoch / cSecondsPerDay);

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


