#include "./Exception.h"

using namespace netlib;

Exception::Exception(std::string what) 
    : _message(what)
{
}