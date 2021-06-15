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
#include <functional>

void dp(int i) {
    ;
}

void dp(char i) {
    ;
}

int main()
{
    int i = 9;
    auto f = std::bind(dp, i);
}