#include "netlib/net/SockAddr.h"
#include "netlib/base/Logging.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testSockAddr) 
{
    netlib::SockAddr addr(1234);
    BOOST_CHECK_EQUAL(addr.toIp(), std::string("0.0.0.0"));
    BOOST_CHECK_EQUAL(addr.toIpPort(), std::string("0.0.0.0:1234"));
    BOOST_CHECK_EQUAL(addr.tpPort(), 1234);

    netlib::SockAddr addr2(1234, true);
    BOOST_CHECK_EQUAL(addr2.toIp(), std::string("127.0.0.1"));
    BOOST_CHECK_EQUAL(addr2.toIpPort(), std::string("127.0.0.1:1234"));
    BOOST_CHECK_EQUAL(addr2.tpPort(), 1234);

    netlib::SockAddr addr3("1.2.3.4", 8888);
    BOOST_CHECK_EQUAL(addr3.toIp(), std::string("1.2.3.4"));
    BOOST_CHECK_EQUAL(addr3.toIpPort(), std::string("1.2.3.4:8888"));
    BOOST_CHECK_EQUAL(addr3.tpPort(), 8888);
}

BOOST_AUTO_TEST_CASE(testSockAddrResolve)
{
    netlib::SockAddr addr(80);
    if(netlib::SockAddr::resolve("baidu.com", &addr)) {
        LOG_INFO << "baidu.com resolve to " << addr.toIpPort();
    }
    else {
        LOG_ERROR << "unable to resolve to baidu.com";
    }
}