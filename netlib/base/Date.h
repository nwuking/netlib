#ifndef DATE_H_
#define DATE_H_

#include <algorithm>
#include <string>

struct tm;

namespace netlib
{

class Date
{
public:

    struct YearMonthDay
    {
        int year;       /// [1900-2500]
        int month;      /// [1-12]
        int day;        /// [1-31]
    };

    static const int cDaysPerWeek = 7;
    static const int cJulianDayOf1970_01_01;

    Date() : _julianDayNumber(0) {}

    Date(int year, int month, int day);

    explicit Date(int julianNumber) : _julianDayNumber(julianNumber) {}

    explicit Date(const struct tm&);

    ~Date() = default;

    void swap(Date &that) {
        std::swap(_julianDayNumber, that._julianDayNumber);
    }

    bool valid() {
        return _julianDayNumber > 0;
    }

    std::string toIsoString() const;

    struct YearMonthDay yearMonthDay() const;


    int year() const {
        return yearMonthDay().year;
    }

    int month() const {
        return yearMonthDay().month;
    }

    int day() const {
        return yearMonthDay().day;
    }

    int weekDay() const {
        /// [0,1,2,3,4,5,6] -> (日，一，... ，五，六)
        return (_julianDayNumber +1) % cDaysPerWeek;
    }

    int julianDayNumber() const {
        return _julianDayNumber;
    }

private:
    int _julianDayNumber;
};

inline bool operator<(Date x, Date y) {
    return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y) {
    return x.julianDayNumber() == y.julianDayNumber();
}

}

#endif