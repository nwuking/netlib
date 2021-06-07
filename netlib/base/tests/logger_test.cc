#include "netlib/base/AsyncLogging.h"
#include "netlib/base/Logging.h"
#include "netlib/base/Time.h"

#include <string>
#include <iostream>

#include <unistd.h>
#include <sys/resource.h>

off_t kRollSize = 500 * 1000 * 1000;

netlib::AsyncLogging *g_asyncLogging = NULL;

void asyncOuput(const char *msg, int len) {
    g_asyncLogging->append(msg, len);
}

void bench(bool longlog) {
    netlib::Logger::setOutput(asyncOuput);

    int cnt = 0;
    const int Banth = 1000;
    std::string empty = " ";
    std::string logStr(3000, 'X');
    logStr += " ";

    for(int t = 0; t < 30; ++t) {
        netlib::Time start = netlib::Time::now();
        for(int i = 0; i < Banth; ++i) {
            LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqstryz "
                     << (longlog ? logStr : empty) << cnt;
            ++cnt;
        }
        netlib::Time end = netlib::Time::now();
        std::cout << netlib::timeDifference(end, start)*1000000/Banth << "\n";
        struct timespec ts = {0, 500*1000*1000};
        nanosleep(&ts, NULL);
    }
}

int main(int argc, char **argv) {
    {
        // set max virtual memory to 2GB
        size_t kOneGB = 1000*1024*1024;
        rlimit rl = {2*kOneGB, 2*kOneGB};
        ::setrlimit(RLIMIT_AS, &rl);
    }

    std::cout << ::getpid() << "\n";
    char name[256] = {0};
    strncpy(name, argv[0], sizeof name-1);
    netlib::AsyncLogging log(::basename(name), kRollSize);
    log.start();
    g_asyncLogging = &log;
    bool longlog = argc > 1;
    bench(longlog);
    return 0;
}