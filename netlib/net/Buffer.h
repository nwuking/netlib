#ifndef BUFFER_H_
#define BUFFER_H_

/*
 *  定义一个缓存区对象结构，用于接发数据消息
 */

#include <vector>
#include <algorithm>
#include <string>

#include <sys/types.h>
#include <assert.h>
#include <endian.h>
#include <stdint.h>
#include <string.h>

namespace netlib
{

class Buffer
{
public:
    static const size_t cInitialSize = 1024;
    static const size_t cCheapPrepend = 8;

    explicit Buffer(size_t initialSize = cInitialSize);
    //~Buffer() = default;

    void swap(Buffer &that) {
        _buffer.swap(that._buffer);
        std::swap(_readerIndex, that._readerIndex);
        std::swap(_writerIndex, that._writerIndex);
    }

    std::string toString() const {
        return std::string(peek(), static_cast<int>(readAbleBytes()));
    }

    size_t readAbleBytes() const {
        return _writerIndex - _readerIndex;
    }

    size_t writeAbleBytes() const {
        return _buffer.size() - _writerIndex;
    }

    size_t prependAbleBytes() const {
        return _readerIndex;
    }

    void ensureWriteAbleBytes(size_t len) {
        if(writeAbleBytes() < len) {
            /// 扩充缓冲区
            makeSpace(len);
        }
        assert(writeAbleBytes() >= len);
    }

    const char* findEOL() const {
        const void *eol = ::memchr(peek(), '\n', readAbleBytes());
        return static_cast<const char*>(eol);
    }

    const char* findEOL(const char *start) const {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const void *eol = ::memchr(start, '\n', beginWrite() - start);
        return static_cast<const char*>(eol);
    }

    void append(const std::string &str) {
        append(str.c_str(), str.size());
    }

    void append(const char *data, size_t len) {
        ensureWriteAbleBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    void append(const void *data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

    void appendInt64(int64_t x) {
        int64_t be64 = ::htobe64(x);
        append(&be64, sizeof be64);
    }

    void appendInt32(int32_t x) {
        int32_t be32 = ::htobe32(x);
        append(&be32, sizeof be32);
    }

    void appendInt16(int16_t x) {
        int16_t be16 = ::htobe16(x);
        append(&be16, sizeof be16);
    }

    void appendInt8(int8_t x) {
        append(&x, sizeof x);
    }

    int64_t readInt64() {
        int64_t ret = peekInt64();
        retrieveInt64();
        return ret;
    }

    int64_t readInt32() {
        int32_t ret = peekInt32();
        retrieveInt32();
        return ret;
    }

    int16_t readInt16() {
        int16_t ret = peekInt16();
        retrieveInt16();
        return ret;
    }

    int8_t readInt8() {
        int8_t ret = peekInt8();
        retrieveInt8();
        return ret;
    }

    char* beginWrite() {
        return begin() + _writerIndex;
    }

    const char* beginWrite() const {
        return begin() + _writerIndex;
    }

    void hasWritten(size_t len) {
        assert(len <= writeAbleBytes());
        _writerIndex += len;
    }

    void unWrite(size_t len) {
        assert(len <= readAbleBytes());
        _writerIndex -= len;
    }

    const char* peek() const {
        return begin() + _readerIndex;
    }

    int64_t peekInt64() const {
        assert(readAbleBytes() >= sizeof(int64_t));
        int64_t be64 = 0;
        ::memcpy(&be64, peek(), sizeof be64);
        return ::be64toh(be64);
    }

    int32_t peekInt32() const {
        assert(readAbleBytes() >= sizeof(int32_t));
        int32_t be32 = 0;
        ::memcpy(&be32, peek(), sizeof be32);
        return ::be32toh(be32);
    }

    int16_t peekInt16() const {
        assert(readAbleBytes() >= sizeof(int16_t));
        int16_t be16 = 0;
        ::memcpy(&be16, peek(), sizeof be16);
        return ::be16toh(be16);
    }

    int8_t peekInt8() const {
        assert(readAbleBytes() >= sizeof(int8_t));
        int8_t x = *peek();
        return x;
    }

    void retrieve(size_t n) {
        assert(n <= readAbleBytes());
        if(n < readAbleBytes()) {
            _readerIndex += n;
        }
        else {
            retrieveAll();
        }
    }

    void retrieveUntil(const char *end) {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
    }

    void retrieveInt64() {
        retrieve(sizeof(int64_t));
    }

    void retrieveInt32() {
        retrieve(sizeof(int32_t));
    }

    void retrieveInt16() {
        retrieve(sizeof(int16_t));
    }

    void retrieveInt8() {
        retrieve(sizeof(int8_t));
    }

    void retrieveAll() {
        _readerIndex = cCheapPrepend;
        _writerIndex = cCheapPrepend;
    }

    std::string retrieveAllAsString() {
        return retrieveAsString(readAbleBytes());
    }

    std::string retrieveAsString(size_t len) {
        assert(len <= readAbleBytes());
        std::string ret(peek(), len);
        retrieve(len);
        return ret;
    }

    void prependInt64(int64_t x) {
        int64_t be64 = ::htobe64(x);
        prepend(&be64, sizeof be64);
    }

    void prependInt32(int32_t x) {
        int32_t be32 = ::htobe32(x);
        prepend(&be32, sizeof be32);
    }

    void prependInt16(int16_t x) {
        int16_t be16 = ::htobe16(x);
        prepend(&be16, sizeof be16);
    }

    void prependInt8(int8_t x) {
        prepend(&x, sizeof(int8_t));
    }

    void prepend(const void *data, size_t len) {
        assert(len <= prependAbleBytes());
        _readerIndex -= len;
        const char *d = static_cast<const char*>(data);
        std::copy(d, d+len, begin()+_readerIndex);
    }

    void shrink(size_t reserve) {
        Buffer other;
        other.ensureWriteAbleBytes(readAbleBytes() + reserve);
        other.append(toString());
        swap(other);
    }

    size_t internalCapcity() const {
        return _buffer.capacity();
    }

    ssize_t readFd(int fd, int *savedErrno);

private:
    char* begin() {
        return &*_buffer.begin();
    }

    const char* begin() const{
        return &*_buffer.begin();
    }

    void makeSpace(size_t len);

    std::vector<char> _buffer;
    int _readerIndex;
    int _writerIndex;
};


}


#endif