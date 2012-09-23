#ifndef CODEC_MP3_HUFFMAN_H_INCLUDED
#define CODEC_MP3_HUFFMAN_H_INCLUDED

#include <bitstream.h>

class HuffmanDecoder
{
    public:
	static void Decode(BitStream& bs, unsigned tableIndex, unsigned& x, unsigned& y);

    private:
	struct HuffmanData
	{
	    // type of the huffman data (0=tree node, 1=real data)
	    unsigned m_type;
	    // 'x' in case of real data
	    unsigned m_value1;
	    // 'y' in case of real data
	    unsigned m_value2;
	};

	static const HuffmanData s_table1[7];
	static const HuffmanData s_table2[17];

	struct HuffmanTable
	{
	    // the actual data of the huffman table
	    const HuffmanData* m_data;
	};

	static const unsigned s_huffmanTableCount = 3; // for now ...
	static const HuffmanTable s_huffmanTables[s_huffmanTableCount];
};

#endif
