#ifndef MP3CODEC_H_INCLUDED
#define MP3CODEC_H_INCLUDED

#include "framesync.h"

#include "../audiocodec.h"
#include "../../bitstream.h"

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

class Mp3Codec : public AudioCodec
{
    public:
	Mp3Codec(DataSource* source);

	bool ReadFrame();

    private:
	/// Reads the data of the frame into the memory
	bool ReadFrameData();

	void ParseSideInformation(const std::vector<unsigned char>& data);
	void ParseMainData(BitStream& bs);

	/// Dumps the contents of the side information table to the standard output
	void DumpSideInformation() const;
	/// Dumps the scale factors
	void DumpScaleFactors() const;
	/// Dumps the huffman decoded samples
	void DumpSamples() const;

	struct SideInformation
	{
	    // possible values from m_blockType member of Granule structure
	    enum BlockType
	    {
		FORBIDDEN,
		START,
		THREE_SHORT_WINDOWS,
		END
	    };

	    struct Granule
	    {
		unsigned m_par2_3Length;
		unsigned m_bigValues;
		unsigned m_globalGain;
		unsigned m_scalefacCompress;
		unsigned m_windowsSwitchingFlag;
		unsigned m_blockType;
		unsigned m_mixedBlockFlag;
		unsigned m_tableSelect[3];
		unsigned m_subBlockGain;
		unsigned m_region0Count;
		unsigned m_region1Count;
		unsigned m_preFlag;
		unsigned m_scalfacScale;
		unsigned m_count1tableSelect;
	    };

	    unsigned m_mainDataBegin;
	    unsigned m_scfsi[2];
	    Granule m_granules[2 /* granule index */][2 /* channel index */];
	};

	codec::mp3::FrameSynchronizer m_synchronizer;
	SideInformation m_sideInformation;

	// scale factors
	// (first idx: granule, second idx: channel)
	std::vector<unsigned> m_scaleFactors[2][2];

	// samples decoded from the huffman data of the current granule
	std::vector<int> m_samples;

	std::vector<unsigned char> m_frameData;
};

#endif
