#include <boost/test/unit_test.hpp>

#include "fakedatasource.h"

#include <codec/mp3/framesync.h>

using codec::mp3::FrameSynchronizer;

static const uint8_t s_mp3Stream[] = {
    // garbage
    0x11, 0x22, 0x33, 0x44, 0x55,
    // valid MP3 frame header
    0xff, 0xfb, 0x90, 0x00,
    // some MP3 frame data ...
    0x00, 0x00, 0x00,
    // valid MP3 frame header
    0xff, 0xfb, 0x90, 0x00,
    // frame data
    0x00, 0x00
};

BOOST_AUTO_TEST_CASE(TestFrameSynchronization)
{
    // initialize input data
    std::vector<unsigned char> data;
    data.insert(data.end(), s_mp3Stream, s_mp3Stream + sizeof(s_mp3Stream));
    test::FakeDataSource source(data);

    FrameSynchronizer sync(source);
    BOOST_CHECK_EQUAL(source.GetPosition(), 0);

    // synchronize to the first frame
    BOOST_CHECK(sync.Sync());
    BOOST_CHECK_EQUAL(source.GetPosition(), 9);

    // synchronize to the next frame
    BOOST_CHECK(sync.Sync());
    BOOST_CHECK_EQUAL(source.GetPosition(), 16);

    // synchronization to the third frame should fail as it is not present
    BOOST_CHECK(!sync.Sync());
}
