#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <string>

namespace netlib
{

class Exception : public std::exception
{
public:
    Exception(std::string what);
    ~Exception() noexcept override = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }
/*
    const char* stackTrace() const noexcept {
        return _stack.c_str();
    }
*/
private:
    std::string _message;
   // std::string _stack;
};

}

#endif