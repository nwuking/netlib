/*
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testk)
{
    BOOST_CHECK_EQUAL(1,1);
}
*/

#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main()
{
    struct timespec tp1;
    struct timespec tp2;
    struct timespec tp3;

    clock_gettime(CLOCK_MONOTONIC, &tp1);
    clock_gettime(CLOCK_REALTIME, &tp2);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp3);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    std::cout << "tp1: " << tp1.tv_sec << " : " << tp1.tv_nsec << "\n";
    std::cout << "tp2: " << tp2.tv_sec << " : " << tp2.tv_nsec << "\n";
    std::cout << "tp3: " << tp3.tv_sec << " : " << tp3.tv_nsec << "\n";

    std::cout << "\ntv: " << tv.tv_sec << " : " << tv.tv_usec << "\n";
}