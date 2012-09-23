#include "huffman.h"

#include <cassert>

// The following tables are used for huffman decoding.
// Script called huffman.py is used to generate them.

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table1[7] =
{
    {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 1, 1}, {1, 0, 1}, {1, 1, 0}, {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table2[17] =
{
    {0, 1, 16}, {0, 1, 12}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 2, 2}, {1, 0, 2}, {1, 1, 2}, {0, 1, 2},
    {1, 2, 1}, {1, 2, 0}, {1, 1, 1}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0}, {1, 0, 0}
};

const HuffmanDecoder::HuffmanTable HuffmanDecoder::s_huffmanTables[HuffmanDecoder::s_huffmanTableCount] =
{
    {NULL},
    {s_table1},
    {s_table2}
};

// =====================================================================================================================
void HuffmanDecoder::Decode(BitStream& bs, unsigned tableIndex, unsigned& x, unsigned& y)
{
    assert(tableIndex < s_huffmanTableCount);
    const HuffmanTable& table = s_huffmanTables[tableIndex];

    // handle special tables: 1st table is empty and 4th is not used
    if (table.m_data == NULL)
    {
	x = 0;
	y = 0;
	return;
    }

    unsigned pos = 0;
    const HuffmanData* data = &table.m_data[pos];

    do
    {
	unsigned bit = bs.GetData(1);

	pos += (bit == 0 ? data->m_value1 : data->m_value2);
	data = &table.m_data[pos];
    } while (data->m_type == 0);

    x = data->m_value1;
    y = data->m_value2;
}
