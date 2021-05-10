#ifndef TIMEZONE_H_
#define TIMEZONE_H_

#include <memory>

#include <time.h>

namespace netlib
{


class TimeZone
{
public:
    TimeZone() = default;
    explicit TimeZone(const char *zonefile);
    TimeZone(int eastOfUtc, const char *tzname);
    ~TimeZone();

    bool vaild() const {
        return static_cast<bool>(_data);
    }

    struct tm toLocalTime(time_t secondsSinceEpoch) const;
    time_t fromLocalTime(const struct tm&) const;

    struct tm toUtcTime(time_t secondsSinceEpoch);
    time_t fromUtcTime(const struct tm&);
    time_t froUtcTime(int year, int month, int day, int hour, int minute, int seconds);

    struct Data;
private:
    std::shared_ptr<Data> _data;
};


}

#endif