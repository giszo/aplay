#include <boost/test/unit_test.hpp>

#include <bitstream.h>

BOOST_AUTO_TEST_CASE(TestEmptyBitStream)
{
    std::vector<unsigned char> data;

    BitStream bs(data.begin(), data.end());
    BOOST_CHECK_EQUAL(bs.GetPosition(), 0);
    BOOST_CHECK_EQUAL(bs.GetLength(), 0);
}

BOOST_AUTO_TEST_CASE(TestGetDataOneByte)
{
    std::vector<unsigned char> data;
    data.push_back(0xea); /* 1110 1010 */

    BitStream bs(data.begin(), data.end());

    // check bitstream before asking any data
    BOOST_CHECK_EQUAL(bs.GetPosition(), 0);
    BOOST_CHECK_EQUAL(bs.GetLength(), 8);

    // ask 2 bits
    BOOST_CHECK_EQUAL(bs.GetData(2), 0x3 /* 11 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 2);

    // ask 3 bits
    BOOST_CHECK_EQUAL(bs.GetData(3), 0x5 /* 101 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 5);

    // ask 1 bit
    BOOST_CHECK_EQUAL(bs.GetData(1), 0x0 /* 0 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 6);

    // ask the remaining 2 bits
    BOOST_CHECK_EQUAL(bs.GetData(2), 0x2 /* 10 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 8);
}

BOOST_AUTO_TEST_CASE(TestGetDataMultipleBytes)
{
    std::vector<unsigned char> data;
    data.push_back(0xe0); /* 1110 0000 */
    for (unsigned i = 0; i < 3; ++i)
	data.push_back(0xaa); /* 1010 1010 */
    data.push_back(0x04); /* 0000 0100 */

    BitStream bs(data.begin(), data.end());

    // check bitstream before asking data
    BOOST_CHECK_EQUAL(bs.GetPosition(), 0);
    BOOST_CHECK_EQUAL(bs.GetLength(), 40);

    // ask 3 bits
    BOOST_CHECK_EQUAL(bs.GetData(3), 0x7 /* 111 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 3);

    // ask 32 bits
    // stream: 0000 0101 0101 0101 0101 0101 0101 0000
    BOOST_CHECK_EQUAL(bs.GetData(32), 0x05555550);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 35);

    // ask the remaining 5 bits
    BOOST_CHECK_EQUAL(bs.GetData(5), 0x4 /* 100 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 40);
}

BOOST_AUTO_TEST_CASE(TestSkipAndRewind)
{
    std::vector<unsigned char> data;
    data.push_back(0x0f); /* 0000 1111 */
    data.push_back(0x00); /* 0000 0000 */
    data.push_back(0x0a); /* 0000 1010 */

    BitStream bs(data.begin(), data.end());

    // skip the first 4 bits
    bs.Skip(4);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 4);

    // get 4 bits
    BOOST_CHECK_EQUAL(bs.GetData(4), 0xf /* 1111 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 8);

    // skip 2 bytes
    bs.Skip(16);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 24);

    // rewind by 3 bits
    bs.Rewind(3);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 21);

    // get the last 3 bits of the stream
    BOOST_CHECK_EQUAL(bs.GetData(3), 0x2 /* 010 */);
    BOOST_CHECK_EQUAL(bs.GetPosition(), 24);
}

BOOST_AUTO_TEST_CASE(TestBitStreamErrors)
{
    std::vector<unsigned char> data;
    data.push_back(0x00);

    BitStream bs(data.begin(), data.end());

    // get 3 bits
    bs.GetData(3);

    // try to ask too much data
    BOOST_CHECK_THROW(bs.GetData(12), BitStreamException);
    // try skip error
    BOOST_CHECK_THROW(bs.Skip(42), BitStreamException);
    // try rewind error
    BOOST_CHECK_THROW(bs.Rewind(42), BitStreamException);
}
