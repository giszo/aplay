#ifndef CODEC_MP3_HUFFMAN_H_INCLUDED
#define CODEC_MP3_HUFFMAN_H_INCLUDED

#include <bitstream.h>

class HuffmanDecoder
{
    public:
	static void DecodeBigValueData(BitStream& bs, unsigned tableIndex, std::vector<int>& samples);
	static void DecodeCount1Data(BitStream& bs, unsigned tableIndex, std::vector<int>& samples);

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
	    unsigned m_linbits;
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
	static const HuffmanData s_table3[17];
	static const HuffmanData s_table5[31];
	static const HuffmanData s_table6[31];
	static const HuffmanData s_table7[71];
	static const HuffmanData s_table8[71];
	static const HuffmanData s_table9[71];
	static const HuffmanData s_table10[127];
	static const HuffmanData s_table11[127];
	static const HuffmanData s_table12[127];
	static const HuffmanData s_table13[511];
	static const HuffmanData s_table15[511];
	static const HuffmanData s_table16[511];
	static const HuffmanData s_table24[511];

	static const HuffmanDataCount1 s_count1table0[31];
	static const HuffmanDataCount1 s_count1table1[31];

	static const unsigned s_bigValueHuffmanTableCount = 32;
	static const HuffmanTable s_bigValueHuffmanTables[s_bigValueHuffmanTableCount];

	static const unsigned s_count1HuffmanTableCount = 2;
	static const HuffmanTableCount1 s_count1HuffmanTables[s_count1HuffmanTableCount];
};

#endif
