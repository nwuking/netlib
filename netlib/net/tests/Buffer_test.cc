#include "netlib/net/Buffer.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(testBufferAppendRetrive)
{
    netlib::Buffer buf;

    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);

    const std::string str(200, 'x');
    buf.append(str);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), str.size());
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize - str.size());
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);

    const std::string str2 = buf.retrieveAsString(50);
    BOOST_CHECK_EQUAL(str2.size(), 50);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), str.size()-str2.size());
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-str.size());
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend+str2.size());
    BOOST_CHECK_EQUAL(str2, std::string(50, 'x'));

    buf.append(str);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 2*str.size()-str2.size());
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-2*str.size());
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend+str2.size());

    const std::string str3 = buf.retrieveAllAsString();
    BOOST_CHECK_EQUAL(str3.size(), 350);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);
    BOOST_CHECK_EQUAL(str3, std::string(350, 'x'));
}

BOOST_AUTO_TEST_CASE(testBufferGrow) 
{
    netlib::Buffer buf;
    buf.append(std::string(400, 'y'));
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 400);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-400);

    buf.retrieve(50);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 350);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-400);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend+50);

    buf.append(std::string(1000, 'z'));
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 1350);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend+50);

    buf.retrieveAll();
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), 1400);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);
}

BOOST_AUTO_TEST_CASE(testBufferInsideFrow)
{
    netlib::Buffer buf;

    buf.append(std::string(800, 'y'));
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 800);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-800);

    buf.retrieve(500);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 300);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-800);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend+500);

    buf.append(std::string(300, 'z'));
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 600);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-600);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);
}

BOOST_AUTO_TEST_CASE(testBufferShrink)
{
    netlib::Buffer buf;
    buf.append(std::string(2000, 'y'));
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 2000);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);

    buf.retrieve(1500);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 500);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend+1500);

    buf.shrink(0);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 500);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize-500);
    BOOST_CHECK_EQUAL(buf.retrieveAllAsString(), std::string(500, 'y'));
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);
}

BOOST_AUTO_TEST_CASE(testBufferPrepend)
{
    netlib::Buffer buf;

    buf.append(std::string(200, 'y'));
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 200);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize - 200);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend);

    int x = 0;
    buf.prepend(&x, sizeof x);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 204);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize - 200);
    BOOST_CHECK_EQUAL(buf.prependAbleBytes(), netlib::Buffer::cCheapPrepend - 4);
}

BOOST_AUTO_TEST_CASE(testBufferReadInt)
{
    netlib::Buffer buf;
    buf.append("HTTP");

    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 4);
    BOOST_CHECK_EQUAL(buf.peekInt8(), 'H');

    int top16 = buf.peekInt16();
    BOOST_CHECK_EQUAL(top16, 'H'*256+'T');
    BOOST_CHECK_EQUAL(buf.peekInt32(), top16*65536+'T'*256+'P');

    BOOST_CHECK_EQUAL(buf.readInt8(), 'H');
    BOOST_CHECK_EQUAL(buf.readInt16(), 'T'*256+'T');
    BOOST_CHECK_EQUAL(buf.readInt8(), 'P');
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 0);
    BOOST_CHECK_EQUAL(buf.writeAbleBytes(), netlib::Buffer::cInitialSize);

    buf.appendInt8(-1);
    buf.appendInt16(-2);
    buf.appendInt32(-3);
    BOOST_CHECK_EQUAL(buf.readAbleBytes(), 7);
    BOOST_CHECK_EQUAL(buf.readInt8(), -1);
    BOOST_CHECK_EQUAL(buf.readInt16(), -2);
    BOOST_CHECK_EQUAL(buf.readInt32(), -3);
}

BOOST_AUTO_TEST_CASE(testBufferFindEol) 
{
    netlib::Buffer buf;
    buf.append(std::string(100000, 'x'));
    const char *null = NULL;
    BOOST_CHECK_EQUAL(buf.findEOL(), null);
    BOOST_CHECK_EQUAL(buf.findEOL(buf.peek()+90000), null);
}

void output(netlib::Buffer &&buf, const void *inner) {
    netlib::Buffer newBuf(std::move(buf));
    BOOST_CHECK_EQUAL(inner, newBuf.peek());
}

BOOST_AUTO_TEST_CASE(testBufferMove)
{
    netlib::Buffer buf;
    buf.append("netlib", 6);
    const void *inner = buf.peek();
    output(std::move(buf), inner);
}