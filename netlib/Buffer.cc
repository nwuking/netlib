#include "./Buffer.h"
#include "./SockFunc.h"

#include <assert.h>
#include <sys/uio.h>

using namespace netlib;


Buffer::Buffer(size_t initialSize)
    : _buffer(cCheapPrepend + cInitialSize),
      _readerIndex(cCheapPrepend),
      _writerIndex(cCheapPrepend)
{
    assert(readAbleBytes() == 0);
    assert(writeAbleBytes() == cInitialSize);
    assert(prependAbleBytes() == cCheapPrepend);
}

ssize_t Buffer::readFd(int fd, int *savedErrno) {
    /// fd-> 为epoll中监听的描述符
    /// 从fd读取数据

    /// 事先准备一个额外的缓存区，防止writeAbleBytes() 不足以写下一条消息
    /// 65535字节为一个IP数据包的最大长度，再加上一个字节的"\n"(65536)
    /// 调用readv(),使用scatter read

    /// struct iovec {
    ///     void *iov_base;         // starting address
    ///     size_t iov_len;         // number of bytes to transfer
    /// };

    char extrabuf[65536];
    size_t writeAble = writeAbleBytes();
    struct iovec vec[2];
    vec[0].iov_base = begin() + _writerIndex;
    vec[0].iov_len = writeAble;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcnt = (writeAble < sizeof extrabuf) ? 2 : 1;

    const ssize_t n = netlib::readv(fd, vec, iovcnt);

    if(n < 0) {
        /// error
        *savedErrno = errno;
    }
    else if(n <= writeAble) {
        /// 当前的缓冲区的可写区域能够当前的消息
        _writerIndex += n;
    }
    else {
        /// 当前的缓冲区的可写区域无法写下一条消息
        /// 缓冲区需要扩容
        _writerIndex = _buffer.size();
        append(extrabuf, n - writeAble);
    }

    return n;
}

void Buffer::makeSpace(size_t len) {
    /// 扩充缓冲区的大小，使其能够写下至少len个字节的数据
    /// 当前缓冲区能写的区域为->prependAbleBytes()+writeAbleBytes

    if(writeAbleBytes() + prependAbleBytes() < len + cCheapPrepend) {
        /// 缓冲区头部的cCheapPrepend字节空间需要预留出来
        /// 需要扩充缓冲区的大小
        _buffer.resize(_writerIndex+len);
    }
    else {
        /// 缓冲区能写的区域能够写下len个字节

        assert(cCheapPrepend < _readerIndex);
        size_t readAble = readAbleBytes();

        /// 将可读数据移动到头部，cCheapPrepend个字节需要保留出来
        std::copy(begin()+_readerIndex, begin()+_writerIndex, begin()+cCheapPrepend);
        _readerIndex = cCheapPrepend;
        _writerIndex = _readerIndex + readAble;
        assert(readAble == readAbleBytes());
    }
}