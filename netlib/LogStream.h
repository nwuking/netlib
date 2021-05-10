#ifndef LOGSTREAM_H_
#define LOGSTREAM_H_

#include <stddef.h>
#include <string.h>

#include <string>

namespace netlib
{
const int cSmallBuffer = 4000;
const int cLargeBuffer = 4000 * 1000;

/// 用于接收保存日志消息
template<int SIZE>
class FixedBuffer
{
public:
    FixedBuffer() : _cur(_data) {}
    ~FixedBuffer();

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

private:
    const char* end() const {
        return _data + sizeof _data;
    }

    char _data[SIZE];
    char *_cur;
};


class LogStream
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

    //self& operator<<(void *data);
    self& operator<<(const char *data);
    self& operator<<(const unsigned char *data);
    self& operator<<(const std::string &data);
    self& operator<<(const Buffer &data);

private:
    template<typename T>
    void formatInteger(T v);

    Buffer _buffer;

    static const int cMaxNumericSize = 32;
};

}


#endif