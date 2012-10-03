#include <codec/mp3/frameheader.h>

#include <iostream>

using codec::mp3::FrameHeader;

const unsigned FrameHeader::s_bitrates_v1[3][16] =
{
    // Layer I
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0},
    // Layer II
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 380, 0},
    // Layer III
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0}
};

const unsigned FrameHeader::s_sampling_v1[4] = {44100, 48000, 32000, 0};

// =====================================================================================================================
FrameHeader::FrameHeader(const NotInitialized& dummy)
    : m_layer(0),
      m_bitrate(0),
      m_sampling(0),
      m_channelMode(SINGLE_CHANNEL),
      m_crc(false),
      m_padded(false)
{
}

// =====================================================================================================================
FrameHeader FrameHeader::Parse(const uint8_t* data)
{
    // the first byte of the header is 0xff always
    if (data[0] != 0xff)
	throw InvalidFrameHeaderException("first byte is not ff");

    if ((data[1] & 0xf8) != 0xf8)
	throw InvalidFrameHeaderException("frame is not MPEG version 1");

    // layer description
    unsigned layerIdx = (data[1] >> 1) & 0x3;

    if (layerIdx == 0)
	throw InvalidFrameHeaderException("invalid layer description");

    // currently we support layer III only
    unsigned layer = 4 - layerIdx;

    if (layer != 3)
	throw InvalidFrameHeaderException("only layer III supported currently");

    // CRC protection
    bool crcProtected = (data[1] & 1) == 0;

    // bitrate index
    unsigned bitrate = (data[2] >> 4) & 15;

    if (bitrate == 0 || bitrate == 15)
	throw InvalidFrameHeaderException("invalid bitrate index");

    // sampling rate index
    unsigned sampling = (data[2] >> 2) & 3;

    if (sampling == 3)
	throw InvalidFrameHeaderException("invalid sampling index");

    // return the parsed frame header
    return FrameHeader(layer, s_bitrates_v1[layer - 1][bitrate], s_sampling_v1[sampling],
	static_cast<ChannelMode>((data[3] >> 6) & 3), crcProtected, data[2] & 1);
}

// =====================================================================================================================
unsigned FrameHeader::CalculateFrameSize() const
{
    return 144 * m_bitrate * 1000 / m_sampling + (m_padded ? 1 : 0);
}

// =====================================================================================================================
FrameHeader::FrameHeader(unsigned layer, unsigned bitrate, unsigned sampling, ChannelMode chanMode, bool crc, bool padded)
    : m_layer(layer),
      m_bitrate(bitrate),
      m_sampling(sampling),
      m_channelMode(chanMode),
      m_crc(crc),
      m_padded(padded)
{
}

// =====================================================================================================================
std::ostream& codec::mp3::operator<<(std::ostream& s, const FrameHeader& header)
{
    s << "Layer " << header.GetLayer() << std::endl;
    s << "Bitrate: " << header.GetBitrate() << " kbps" << std::endl;
    s << "Sampling rate: " << header.GetSamplingRate() << " Hz" << std::endl;
    s << "CRC protected: " << (header.IsProtected() ? "yes" : "no");
    return s;
}
