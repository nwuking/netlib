#include "TimeZone.h"

#include <vector>
#include <string>

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

}

using namespace netlib;

struct TimeZone::Data {
    std::vector<transition> transitions;
    std::vector<LocalTime> localtimes;
    std::vector<std::string> name;
    std::string abbrviation;
};


