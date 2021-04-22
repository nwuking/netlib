#include "./CurrentThread.h"

namespace netlib
{

namespace CurrentThread
{

__thread int t_cachedTid = 0;

}

}