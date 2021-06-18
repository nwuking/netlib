/*
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testk)
{
    BOOST_CHECK_EQUAL(1,1);
}
*/

#include <iostream>

#include <pthread.h>
#include <deque>

#include <stdio.h>
#include <stdlib.h>


class Queue
{
public:
    Queue();

private:
    phtread_mutex_t _mutex;
    pthread_cond_t _cond;

};