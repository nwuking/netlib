#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

namespace netlib
{

class NonCopyAble
{
public:
    NonCopyAble(const NonCopyAble&) = delete;
    void operator=(const NonCopyAble&) = delete;

protected:
    NonCopyAble() = default;
    ~NonCopyAble() = default;
};

}

#endif