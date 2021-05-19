#ifndef BUFFER_H_
#define BUFFER_H_

/*
 *  定义一个缓存区对象结构，用于接发数据消息
 */

#include <vector>
#include <algorithm>

#include <sys/types.h>
#include <assert.h>

namespace netlib
{

class Buffer
{
public:
    static const size_t cInitialSize = 1024;
    static const size_t cCheapPrepend = 8;

    explicit Buffer(size_t initialSize = cInitialSize);
    //~Buffer() = default;

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

    void append(const char *data, size_t len) {
        ensureWriteAbleBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    void append(const void *data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

    char* beginWrite() {
        return begin() + _writerIndex;
    }

    void hasWritten(size_t len) {
        assert(len <= writeAbleBytes());
        _writerIndex += len;
    }

    const char* peek() const {
        return begin() + _readerIndex;
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

    void retrieveAll() {
        _readerIndex = cCheapPrepend;
        _writerIndex = cCheapPrepend;
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