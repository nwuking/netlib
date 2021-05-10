#include "./LogStream.h"

#include <algorithm>

namespace netlib 
{

const char digits[] = "9876543210123456789";
const char *zero = digits + 9;

template<typename T>
size_t convert(char *buf, T v) {
    /// 将数字转化为字符串
    T i = v;
    char *p = buf;

    do {
        int lsd = static_cast<int>(i%10);
        i /= 10;
        *p++ = zero[lsd];
    } while(i != 0);

    if(v < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

template<typename T>
void LogStream::formatInteger(T v) {
    if(_buffer.vaild() >= cMaxNumericSize) {
        size_t len = convert(_buffer.current(), v);
        _buffer.add(len);
    }
}

LogStream& LogStream::operator<<(bool v) {
    _buffer.append(v ? "1" : "0", 1);
    return *this;
}

LogStream& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(float v) {
    *this << static_cast<double>(v);
    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if(_buffer.vaild() >= cMaxNumericSize) {
        int len = ::snprintf(_buffer.current(), cMaxNumericSize, "%.12g", v);
        _buffer.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(char v) {
    _buffer.append(&v, 1);
    return *this;
}

LogStream& LogStream::operator<<(const char *data) {
    if(data) {
        _buffer.append(data, ::strlen(data));
    }
    else {
        _buffer.append("(null)", 6);
    }
    return *this;
}

LogStream& LogStream::operator<<(const unsigned char *data) {
    return operator<<(reinterpret_cast<const char*>(data));
}

LogStream& LogStream::operator<<(const std::string &data) {
    _buffer.append(data.c_str(), data.size());
    return *this;
}

LogStream& LogStream::operator<<(const Buffer &data) {
    *this << data.toString();
    return *this;
}

}