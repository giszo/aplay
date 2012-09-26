/**
 * Documents used to implement this MP3 decoder:
 *   - http://www.mp3-tech.org/programmer/docs/mp3_theory.pdf
 *   - http://www.mp3-tech.org/programmer/docs/iso11172-3.zip
 *   - http://www.mp3-tech.org/programmer/docs/bitstream.zip
 */

#include "huffman.h"

#include <codec/mp3.h>
#include <bitstream.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cassert>
#include <cstdlib>

const std::string Mp3Codec::s_layerNames[5] = {"", "I", "II", "III", "reserved"};

// Bitrate table for MPEG version 1 Layer III
const unsigned Mp3Codec::s_bitrates_v1[3][16] =
{
    // Layer I
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0},
    // Layer II
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 380, 0},
    // Layer III
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0}
};

// Sampling rate for MPEG version 1
const unsigned Mp3Codec::s_sampling_v1[4] = {44100, 48000, 32000, 0};

// Channel mode descriptions
const std::string Mp3Codec::s_channelModes[4] = {"Stereo", "Joint stereo", "Dual channel", "Single channel"};

// =====================================================================================================================
Mp3Codec::Mp3Codec(DataSource* source)
    : AudioCodec(source)
{}

// =====================================================================================================================
bool Mp3Codec::ReadFrame()
{
    if (!SyncFrame())
    {
	std::cerr << "mp3: frame synchronization failed" << std::endl;
	return false;
    }

    DumpHeader();
    ReadFrameData();

    return true;
}

// =====================================================================================================================
bool Mp3Codec::SyncFrame()
{
    // Read the whole header
    if (m_source->Read(m_header, 4) != 4)
	return false;

    while (1)
    {
	// Check if we have a valid frame header
	if (IsHeaderValid())
	{
	    std::cout << "mp3: found frame at " << (m_source->GetPosition() - 4) << std::endl;
	    return true;
	}

	// Frame header is invalid, so drop the first byte, shift the remaining left, read a new byte and try again ...
	for (unsigned i = 0; i < 3; ++i)
	    m_header[i] = m_header[i + 1];

	if (m_source->Read(&m_header[3], 1) != 1)
	    return false;
    }
}

// =====================================================================================================================
bool Mp3Codec::ReadFrameData()
{
    std::vector<unsigned char> data;

    // calculate frame size
    unsigned frameSize = 144 * GetBitrate() * 1000 / GetSamplingRate() + (IsPadded() ? 1 : 0);

    // CRC protection
    if (IsCrcProtected())
    {
	uint16_t crc;
	if (m_source->Read(&crc, 2) != 2)
	    return false;

	// TODO: implement CRC validation
    }

    // read side information
    unsigned sideInfoSize = IsMono() ? 17 : 32;
    data.resize(sideInfoSize);
    if (m_source->Read(&data[0], sideInfoSize) != static_cast<int>(sideInfoSize))
	return false;

    // parse side information
    ParseSideInformation(data);
//    DumpSideInformation();

    // calculate the size of the main data in the current frame
    unsigned frameMainDataSize =
	frameSize - (4 /* header size */ + (IsCrcProtected() ? 2 : 0) /* crc value */ + sideInfoSize /* side info size */);

    // load the data from the current frame
    size_t oldDataSize = m_frameData.size();
    m_frameData.resize(oldDataSize + frameMainDataSize);

    if (m_source->Read(&m_frameData[oldDataSize], frameMainDataSize) != static_cast<int>(frameMainDataSize))
	return false;

    // parse main data
    BitStream mainDataStream(
	m_frameData.begin() + oldDataSize - m_sideInformation.m_mainDataBegin,
	m_frameData.end());
    ParseMainData(mainDataStream);

    return true;
}

// =====================================================================================================================
void Mp3Codec::ParseSideInformation(const std::vector<unsigned char>& data)
{
    BitStream bs(data.begin(), data.end());

    // main data begin
    m_sideInformation.m_mainDataBegin = bs.GetData(9);
    // private bits
    bs.GetData(IsMono() ? 5 : 3);
    // scfsi
    for (unsigned channel = 0; channel < (IsMono() ? 1 : 2); ++channel)
	m_sideInformation.m_scfsi[channel] = bs.GetData(4);
    // granules
    for (unsigned grIndex = 0; grIndex < 2; ++grIndex)
    {
	for (unsigned channel = 0; channel < (IsMono() ? 1 : 2); ++channel)
	{
	    SideInformation::Granule* granule = &m_sideInformation.m_granules[grIndex][channel];

	    granule->m_par2_3Length = bs.GetData(12);
	    granule->m_bigValues = bs.GetData(9);
	    granule->m_globalGain = bs.GetData(8);
	    granule->m_scalefacCompress = bs.GetData(4);
	    granule->m_windowsSwitchingFlag = bs.GetData(1);

	    if (granule->m_windowsSwitchingFlag)
	    {
		granule->m_blockType = bs.GetData(2);
		granule->m_mixedBlockFlag = bs.GetData(1);
		for (unsigned i = 0; i < 2; ++i)
		    granule->m_tableSelect[i] = bs.GetData(5);
		granule->m_tableSelect[2] = 0; // not used here
		granule->m_subBlockGain = bs.GetData(9);

		if (granule->m_blockType == SideInformation::THREE_SHORT_WINDOWS)
		    granule->m_region0Count = 8;
		else
		    granule->m_region0Count = 7;
		granule->m_region1Count = 20 - granule->m_region0Count;
	    }
	    else
	    {
		// block type is not used, it's safe to use 0 as a default value
		// (see Mp3 The Theory Behind - Page 16)
		granule->m_blockType = 0;
		granule->m_mixedBlockFlag = 0;
		for (unsigned i = 0; i < 3; ++i)
		    granule->m_tableSelect[i] = bs.GetData(5);
		granule->m_region0Count = bs.GetData(4);
		granule->m_region1Count = bs.GetData(3);
	    }

	    granule->m_preFlag = bs.GetData(1);
	    granule->m_scalfacScale = bs.GetData(1);
	    granule->m_count1tableSelect = bs.GetData(1);
	}
    }
}

// =====================================================================================================================
static inline bool IsScaleFactorGroupPresent(unsigned granuleIndex, unsigned scfsi, unsigned scalFacGroupIdx)
{
    return
	// for the first granule all the scale factors are present always
	(granuleIndex == 0) ||
	// check scfsi part of the side information for the second granule
	// (zero bit in the scfsi table means that the corresponding scale factor group is present in the granule)
	((scfsi & (1 << (3 - scalFacGroupIdx))) == 0);
}

// =====================================================================================================================
static inline unsigned GetBigValuesRegionIndex(unsigned position, unsigned region1Start, unsigned region2Start)
{
    if (position < region1Start)
	return 0;
    if (position < region2Start)
	return 1;
    return 2;
}

// =====================================================================================================================
void Mp3Codec::ParseMainData(BitStream& bs)
{
    static const unsigned scaleFactorBandIndex[3][23] =
    {
	// 44100 Hz
	{0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162, 196, 238, 288, 342, 418, 576},
	// 48000 Hz
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // TODO
	// 32000 Hz
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // TODO
    };

    for (unsigned grIndex = 0; grIndex < 2; ++grIndex)
    {
	for (unsigned channel = 0; channel < (IsMono() ? 1 : 2); ++channel)
	{
	    SideInformation::Granule* granule = &m_sideInformation.m_granules[grIndex][channel];

	    // Save the start position of part2 so we can calculate its length later on
	    unsigned part2Start = bs.GetPosition();

	    // Scale factor length table
	    // (see Mp3 The Theory Behind - Page 15 - Table 5.11)
	    static const unsigned scaleFactorLength[2][16] =
	    {
		{0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4},
		{0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3}
	    };

	    // Parse the scale factors
	    m_scaleFactors[grIndex][channel].clear();

	    if (granule->m_blockType == SideInformation::THREE_SHORT_WINDOWS)
	    {
		if (granule->m_mixedBlockFlag)
		{
		    std::cout << "mixed" << std::endl;
		    // TODO
		    abort();
		}
		else
		{
		    // Scale factors are divided into 2 groups (0-5, 6-11)
		    static const unsigned scaleFactorCountPerGroup[2] = {6, 6};

		    for (unsigned sfGrpIdx = 0; sfGrpIdx < 2; ++sfGrpIdx)
		    {
			unsigned sfBits = scaleFactorLength[sfGrpIdx][granule->m_scalefacCompress];

			for (unsigned sfCnt = 0; sfCnt < scaleFactorCountPerGroup[sfGrpIdx]; ++sfCnt)
			    for (unsigned cnt = 0; cnt < 3; ++cnt)
				m_scaleFactors[grIndex][channel].push_back(bs.GetData(sfBits));
		    }
		}
	    }
	    else
	    {
		// Scale factors are divided into 4 groups with the following number of items in each group: 6, 5, 5, 5
		// (see Mp3 The Theory Behind - Page 14 - Table 5.9)
		static const unsigned scaleFactorCountPerGroup[4] = {6, 5, 5, 5};

		for (unsigned sfGrpIdx = 0; sfGrpIdx < 4; ++sfGrpIdx)
		{
		    if (IsScaleFactorGroupPresent(grIndex, m_sideInformation.m_scfsi[channel], sfGrpIdx))
		    {
			unsigned sfBits = scaleFactorLength[sfGrpIdx / 2][granule->m_scalefacCompress];

			for (unsigned cnt = 0; cnt < scaleFactorCountPerGroup[sfGrpIdx]; ++cnt)
			    m_scaleFactors[grIndex][channel].push_back(bs.GetData(sfBits));
		    }
		}
	    }

	    // Part2 (the scale factors) are parsed now, it is time to calculate the length of this part
	    unsigned part2Length = bs.GetPosition() - part2Start;
	    // Make sure part2 is not bigger that was written in the side information for part2+3
	    assert(granule->m_par2_3Length >= part2Length);

//	    std::cout << "part2: start=" << part2Start << " length=" << part2Length << std::endl;

	    // Parse the main (huffman coded) data
	    unsigned region1Start;
	    unsigned region2Start;

	    if (granule->m_blockType == SideInformation::THREE_SHORT_WINDOWS)
	    {
		// Start of region1 and region2 is fixed for short windows
		region1Start = 36;
		region2Start = 576;
	    }
	    else
	    {
		assert(granule->m_region0Count + 1 < 23);
		assert(granule->m_region0Count + 1 + granule->m_region1Count + 1 < 23);

		region1Start = scaleFactorBandIndex[0 /* TODO: 44.1kHz for now */][granule->m_region0Count + 1];
		region2Start = scaleFactorBandIndex[0 /* TODO: 44.1kHz for now */][granule->m_region0Count + 1 + granule->m_region1Count + 1];
	    }

	    m_samples.clear();

	    // Parse big values section
	    for (unsigned i = 0; i < granule->m_bigValues * 2; i += 2)
	    {
		unsigned htIndex = granule->m_tableSelect[GetBigValuesRegionIndex(i, region1Start, region2Start)];
		HuffmanDecoder::DecodeBigValueData(bs, htIndex, m_samples);
	    }

	    // Parse count1 area
	    while ((bs.GetPosition() < (part2Start + granule->m_par2_3Length)) && (m_samples.size() < 576))
		HuffmanDecoder::DecodeCount1Data(bs, granule->m_count1tableSelect, m_samples);

	    // Check the position in the bitstream as we may exceed the limit of the part2_3 area.
	    unsigned position = bs.GetPosition();
	    unsigned part2_3End = part2Start + granule->m_par2_3Length;

	    if (position > part2_3End)
	    {
		// Discard the last entry decoded from the count1 area
		for (unsigned i = 0; i < 4; ++i)
		    m_samples.pop_back();

		// ... and rewind the bitstream to be at the end of the current granule
		bs.Rewind(position - part2_3End);
	    }
	    else if (position < part2_3End)
	    {
		// Get the remaining empty bits from the huffman coded data
		bs.Skip(part2_3End - position);
	    }

	    // Make sure we are at the end of the current granule now
	    assert(bs.GetPosition() == part2_3End);

//	    while (m_samples.size() < 576)
//		m_samples.push_back(0);

	    DumpSamples();
	}
    }
}

// =====================================================================================================================
unsigned Mp3Codec::GetLayer() const
{
    return 4 - ((m_header[1] >> 1) & 0x3);
}

// =====================================================================================================================
unsigned Mp3Codec::GetBitrate() const
{
    unsigned layer = GetLayer();

    if (layer != 3)
    {
	std::cerr << "Error: layer " << s_layerNames[layer] << " is not supported!" << std::endl;
	return 0;
    }

    unsigned bitrateIdx = (m_header[2] >> 4) & 15;
    return s_bitrates_v1[layer - 1][bitrateIdx];
}

// =====================================================================================================================
unsigned Mp3Codec::GetSamplingRate() const
{
    unsigned samplingIdx = (m_header[2] >> 2) & 3;
    return s_sampling_v1[samplingIdx];
}

// =====================================================================================================================
bool Mp3Codec::IsHeaderValid() const
{
    // first byte is full of 1s
    if (m_header[0] != 0xff)
	return false;

    // only MPEG Version 1 is supported for now
    if ((m_header[1] & 0xf8) != 0xf8)
	return false;

    // invalid layer description?
    if (((m_header[1] >> 1) & 0x3) == 0)
	return false;

    // invalid bitrate index?
    unsigned brIndex = (m_header[2] >> 4) & 15;
    if (brIndex == 0 || brIndex == 15)
	return false;

    // invalid sampling frequency index?
    if (((m_header[2] >> 2) & 3) == 3)
	return false;

    return true;
}

// =====================================================================================================================
Mp3Codec::ChannelMode Mp3Codec::GetChannelMode() const
{
    return static_cast<ChannelMode>((m_header[3] >> 6) & 3);
}

// =====================================================================================================================
bool Mp3Codec::IsCrcProtected() const
{
    return (m_header[1] & 1) == 0;
}

// =====================================================================================================================
bool Mp3Codec::IsPadded() const
{
    return (m_header[2] & 1);
}

// =====================================================================================================================
void Mp3Codec::DumpHeader() const
{
    std::cout << "MP3 header:" << std::endl;
    printf("  Raw: %02x%02x%02x%02x\n", m_header[0], m_header[1], m_header[2], m_header[3]);
//    std::cout << "  Raw: ";
//    for (unsigned i = 0; i < 4; ++i)
//	std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(m_header[i]);
//    std::cout << std::endl;
    std::cout << "  Layer " << s_layerNames[GetLayer()] << std::endl;
    std::cout << "  CRC: " << (IsCrcProtected() ? "protected" : "not protected") << std::endl;
    std::cout << "  Bitrate: " << GetBitrate() << "kbps" << std::endl;
    std::cout << "  Sampling: " << GetSamplingRate() << "Hz" << std::endl;
    std::cout << "  Padding: " << (IsPadded() ? "yes" : "no") << std::endl;
    std::cout << "  Channel mode: " << s_channelModes[GetChannelMode()] << std::endl;
}

// =====================================================================================================================
void Mp3Codec::DumpSideInformation() const
{
    std::cout << "Side information:" << std::endl;
    std::cout << "  Main data begin: " << m_sideInformation.m_mainDataBegin << std::endl;
    std::cout << "  SCFSI: " << m_sideInformation.m_scfsi[0];
    if (!IsMono()) std::cout << " " <<  m_sideInformation.m_scfsi[1];
    std::cout << std::endl;
    for (unsigned grIndex = 0; grIndex < 2; ++grIndex)
    {
	std::cout << "  Granule #" << grIndex << std::endl;

	for (unsigned channel = 0; channel < (IsMono() ? 1 : 2); ++channel)
	{
	    const SideInformation::Granule* granule = &m_sideInformation.m_granules[grIndex][channel];

	    std::cout << "    Channel #" << channel << std::endl;
	    std::cout << "      Par2_3 length: " << granule->m_par2_3Length << std::endl;
	    std::cout << "      Big values: " << granule->m_bigValues << std::endl;
	    std::cout << "      Global gain: " << granule->m_globalGain << std::endl;
	    std::cout << "      Scalefac compress: " << granule->m_scalefacCompress << std::endl;
	    std::cout << "      Windows switching flag: " << (granule->m_windowsSwitchingFlag ? "set" : "not set") << std::endl;

	    if (granule->m_windowsSwitchingFlag)
	    {
		std::cout << "      Block type: " << granule->m_blockType << std::endl;
		std::cout << "      Mixed block flag: " << granule->m_mixedBlockFlag << std::endl;		
		std::cout << "      Table select: " << granule->m_tableSelect[0] << " " << granule->m_tableSelect[1] << std::endl;
		std::cout << "      Sub block gain: " << granule->m_subBlockGain << std::endl;
	    }
	    else
	    {
		// TODO
	    }

	    std::cout << "      Pre flag: " << granule->m_preFlag << std::endl;
	    std::cout << "      Scalfac scale: " << granule->m_scalfacScale << std::endl;
	    std::cout << "      Count1table select: " << granule->m_count1tableSelect << std::endl;
	}
    }
}

// =====================================================================================================================
void Mp3Codec::DumpScaleFactors() const
{
    std::cout << "Scale factors:" << std::endl;

    for (unsigned grIndex = 0; grIndex < 2; ++grIndex)
    {
	for (unsigned channel = 0; channel < (IsMono() ? 1: 2); ++channel)
	{
	    const std::vector<unsigned>& sfs = m_scaleFactors[grIndex][channel];

	    std::cout << " ";
	    for (std::vector<unsigned>::const_iterator it = sfs.begin(); it != sfs.end(); ++it)
		std::cout << " " << *it;
	    std::cout << std::endl;
	}
    }
}

// =====================================================================================================================
void Mp3Codec::DumpSamples() const
{
    std::cout << "Samples: (count=" << m_samples.size() << ")" << std::endl;

    if (!m_samples.empty())
    {
	std::cout << " ";
	for (std::vector<int>::const_iterator it = m_samples.begin(); it != m_samples.end(); ++it)
	    std::cout << " " << *it;
	std::cout << std::endl;
    }
}
