#include "TimeZone.h"

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

TimeZone::TimeZone(const char *zonefile)
    : _data(new TimeZone::Data)
{
}

TimeZone::TimeZone(int eastOfUtc, const char *tzname) 
    : _data(new TimeZone::Data)
{
    _data->localtimes.push_back(netlib::LocalTime(eastOfUtc, false, 0));
    _data->abbrviation = tzname;
}

struct tm TimeZone::toLocalTime(time_t seconds) const {
    struct tm localTime;
    ::bzero(&localTime, sizeof localTime);
    assert(_data != NULL);
    const Data &data(*_data);

    netlib::transition senry(seconds, 0, 0);
    //const netlib::LocalTime *local = 
}


