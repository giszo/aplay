#include <boost/test/unit_test.hpp>

#include <codec/mp3/frameheader.h>

using codec::mp3::FrameHeader;
using codec::mp3::InvalidFrameHeaderException;

static const uint8_t s_frameHeaderWithoutSyncBits[4] = {0, 0, 0, 0};
static const uint8_t s_frameHeaderMpegV2[4] = {0xff, 0xf3, 0x90, 0x00};
static const uint8_t s_frameHeaderInvalidLayer[4] = {0xff, 0xf9, 0x90, 0x00};
static const uint8_t s_frameHeaderInvalidBitrate[4] = {0xff, 0xfb, 0x00, 0x00};
static const uint8_t s_frameHeaderInvalidSampling[4] = {0xff, 0xfb, 0x9c, 0x00};

// MPEG version 1 Layer III, not protected, 128 kbps, 44100 Hz, not padded, stereo mode
static const uint8_t s_validFrameHeader[4] = {0xff, 0xfb, 0x90, 0x00};

BOOST_AUTO_TEST_CASE(TestFailedFrameHeaderParsing)
{
    // frame header without synchronization bits (no 0xff in first byte)
    BOOST_CHECK_THROW(FrameHeader::Parse(s_frameHeaderWithoutSyncBits), InvalidFrameHeaderException);

    // frame header with MPEG version 2
    BOOST_CHECK_THROW(FrameHeader::Parse(s_frameHeaderMpegV2), InvalidFrameHeaderException);

    // frame with invalid (00) layer index
    BOOST_CHECK_THROW(FrameHeader::Parse(s_frameHeaderInvalidLayer), InvalidFrameHeaderException);

    // frame with invalid (0000) bitrate index
    BOOST_CHECK_THROW(FrameHeader::Parse(s_frameHeaderInvalidBitrate), InvalidFrameHeaderException);

    // frame with invalid (11) sampling rate index
    BOOST_CHECK_THROW(FrameHeader::Parse(s_frameHeaderInvalidSampling), InvalidFrameHeaderException);
}

BOOST_AUTO_TEST_CASE(TestSuccessfulFrameHeaderParsing)
{
    FrameHeader header = FrameHeader::Parse(s_validFrameHeader);

    BOOST_CHECK_EQUAL(header.GetLayer(), 3);
    BOOST_CHECK_EQUAL(header.GetBitrate(), 128);
    BOOST_CHECK_EQUAL(header.GetSamplingRate(), 44100);
    BOOST_CHECK(!header.IsProtected());
}
