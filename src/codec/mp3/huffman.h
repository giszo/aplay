#ifndef CODEC_MP3_HUFFMAN_H_INCLUDED
#define CODEC_MP3_HUFFMAN_H_INCLUDED

#include <bitstream.h>

class HuffmanDecoder
{
    public:
	static void DecodeBigValueData(BitStream& bs, unsigned tableIndex, std::vector<int>& samples);
	static void DecodeCount1Data(BitStream& bs, unsigned tableIndex, std::vector<int>& samples);

    private:
	struct HuffmanData
	{
	    // type of the huffman data (0=tree node, 1=real data)
	    unsigned m_type;
	    // 'x' in case of real data
	    int m_value1;
	    // 'y' in case of real data
	    int m_value2;
	};

	struct HuffmanTable
	{
	    // the actual data of the huffman table
	    const HuffmanData* m_data;
	};

	struct HuffmanDataCount1
	{
	    unsigned m_type;
	    int m_v;
	    int m_w;
	    int m_x;
	    int m_y;
	};

	struct HuffmanTableCount1
	{
	    const HuffmanDataCount1* m_data;
	};

	static const HuffmanData s_table1[7];
	static const HuffmanData s_table2[17];

	static const HuffmanDataCount1 s_count1table0[31];
	static const HuffmanDataCount1 s_count1table1[31];

	static const unsigned s_bigValueHuffmanTableCount = 3; // for now ...
	static const HuffmanTable s_bigValueHuffmanTables[s_bigValueHuffmanTableCount];

	static const unsigned s_count1HuffmanTableCount = 2;
	static const HuffmanTableCount1 s_count1HuffmanTables[s_count1HuffmanTableCount];
};

#endif
