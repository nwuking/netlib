#ifndef PROCESSINFO_H_
#define PROCESSINFO_H_

#include <sys/types.h>

#include <string>

namespace netlib
{

namespace ProcessInfo
{

std::string hostname();

pid_t pid();

}


}


#endif