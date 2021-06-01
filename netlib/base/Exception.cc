#include "netlib/base/Exception.h"
#include "netlib/base/CurrentThread.h"

using namespace netlib;

Exception::Exception(std::string what) 
    : _message(what),
      _stack(CurrentThread::stackTrace(false))
{
}