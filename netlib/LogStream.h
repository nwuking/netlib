#ifndef LOGSTREAM_H_
#define LOGSTREAM_H_

#include "./noncopyable.h"

#include <stddef.h>
#include <string.h>

#include <string>

namespace netlib
{
const int cSmallBuffer = 4000;
const int cLargeBuffer = 4000 * 1000;

/// 用于接收保存日志消息
template<int SIZE>
class FixedBuffer : public NonCopyAble
{
public:
    FixedBuffer() : _cur(_data) {
        setCookie(cookieStart);
    }
    ~FixedBuffer() {
        setCookie(cookieEnd);
    }

    void append(const char *buf, size_t len) {
        if(static_cast<size_t>(vaild()) > len) {
            ::memcpy(_cur, buf, len);
            _cur += len;
        }
    }

    int vaild() const {
        return static_cast<int>(end() - _cur);
    }

    const char* data() const {
        return _data;
    }

    int length() const {
        return static_cast<int>(_cur - _data);
    }

    char* current() {
        return _cur;
    }

    void add(size_t len) {
        _cur += len;
    }

    void reset() {
        _cur = _data;
    }

    void bzero() {
        ::bzero(_data, sizeof _data);
    }

    std::string toString() const {
        return std::string(_data, length());
    }

    void setCookie(void (*cookie)()) {
        _cookie = cookie;
    }

private:
    const char* end() const {
        return _data + sizeof _data;
    }

    static void cookieStart();
    static void cookieEnd();

    void (*_cookie)();

    char _data[SIZE];
    char *_cur;
};


class LogStream : public NonCopyAble
{
    typedef LogStream self;

public:
    typedef netlib::FixedBuffer<netlib::cSmallBuffer> Buffer;

    LogStream();
    ~LogStream();

    /// 重载<<符号
    self& operator<<(bool v);
    self& operator<<(short v);
    self& operator<<(unsigned short v);
    self& operator<<(int v);
    self& operator<<(unsigned int v);
    self& operator<<(long v);
    self& operator<<(unsigned long v);
    self& operator<<(long long v);
    self& operator<<(unsigned long long v);
    self& operator<<(float v);
    self& operator<<(double v);
    self& operator<<(char v);

    self& operator<<(const void *data);
    self& operator<<(const char *data);
    self& operator<<(const unsigned char *data);
    self& operator<<(const std::string &data);
    self& operator<<(const Buffer &data);

    void append(const char *data, int len) {
        _buffer.append(data, len);
    }

    const Buffer& buffer() const {
        return _buffer;
    }

    void resetBuffer() {
        _buffer.reset();
    }

private:
    template<typename T>
    void formatInteger(T v);

    Buffer _buffer;

    static const int cMaxNumericSize = 32;
};

class Fmt
{
public:
    template<typename T>
    Fmt(const char *fmt, T val);

    const char* data() const {
        return _data;
    }

    int length() const {
        return _length;
    }

private:
    char _data[32];
    int _length;
};

inline LogStream& operator<<(LogStream &s, Fmt &fmt) {
    s.append(fmt.data(), fmt.length());
    return s;
}

}


#endif