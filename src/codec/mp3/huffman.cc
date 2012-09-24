#include "huffman.h"

#include <cassert>
#include <cstddef>

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

const HuffmanDecoder::HuffmanDataCount1 HuffmanDecoder::s_count1table0[31] =
{
    {0, 1, 30, 0, 0}, {0, 1, 22, 0, 0}, {0, 1, 14, 0, 0}, {0, 1, 8, 0, 0}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 1, 0, 1, 1}, {1, 1, 1, 1, 1}, {0, 1, 2, 0, 0}, {1, 1, 1, 0, 1},
    {1, 1, 1, 1, 0}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 0, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 0, 0, 1}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 0, 1, 1, 0}, {1, 0, 0, 1, 1},
    {0, 1, 2, 0, 0}, {1, 1, 0, 1, 0}, {1, 1, 1, 0, 0}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 0, 0, 1, 0}, {1, 0, 0, 0, 1}, {0, 1, 2, 0, 0}, {1, 0, 1, 0, 0}, {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0}
};

const HuffmanDecoder::HuffmanDataCount1 HuffmanDecoder::s_count1table1[31] =
{
    {0, 1, 16, 0, 0}, {0, 1, 8, 0, 0}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 0}, {0, 1, 2, 0, 0}, {1, 1, 1, 0, 1}, {1, 1, 1, 0, 0}, {0, 1, 4, 0, 0},
    {0, 1, 2, 0, 0}, {1, 1, 0, 1, 1}, {1, 1, 0, 1, 0}, {0, 1, 2, 0, 0}, {1, 1, 0, 0, 1}, {1, 1, 0, 0, 0}, {0, 1, 8, 0, 0}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 0, 1, 1, 1},
    {1, 0, 1, 1, 0}, {0, 1, 2, 0, 0}, {1, 0, 1, 0, 1}, {1, 0, 1, 0, 0}, {0, 1, 4, 0, 0}, {0, 1, 2, 0, 0}, {1, 0, 0, 1, 1}, {1, 0, 0, 1, 0}, {0, 1, 2, 0, 0}, {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0}
};

const HuffmanDecoder::HuffmanTable HuffmanDecoder::s_bigValueHuffmanTables[HuffmanDecoder::s_bigValueHuffmanTableCount] =
{
    {NULL},
    {s_table1},
    {s_table2}
};

const HuffmanDecoder::HuffmanTableCount1 HuffmanDecoder::s_count1HuffmanTables[HuffmanDecoder::s_count1HuffmanTableCount] = 
{
    {s_count1table0},
    {s_count1table1}
};

// =====================================================================================================================
static int DecodeSign(BitStream& bs, int value)
{
    // no sign bit for zero value
    if (value == 0)
	return value;

    // '1' bit means negative value
    if (bs.GetData(1))
	value = -value;

    return value;
}

// =====================================================================================================================
void HuffmanDecoder::DecodeBigValueData(BitStream& bs, unsigned tableIndex, std::vector<int>& samples)
{
    assert(tableIndex < s_bigValueHuffmanTableCount);
    const HuffmanTable& table = s_bigValueHuffmanTables[tableIndex];

    // handle special tables: 1st table is empty and 4th is not used
    if (table.m_data == NULL)
    {
	samples.push_back(0);
	samples.push_back(0);
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

    // TODO: handle libnits!
    samples.push_back(DecodeSign(bs, data->m_value1));
    samples.push_back(DecodeSign(bs, data->m_value2));
}

// =====================================================================================================================
void HuffmanDecoder::DecodeCount1Data(BitStream& bs, unsigned tableIndex, std::vector<int>& samples)
{
    assert(tableIndex < s_count1HuffmanTableCount);
    const HuffmanTableCount1& table = s_count1HuffmanTables[tableIndex];

    unsigned pos = 0;
    const HuffmanDataCount1* data = &table.m_data[pos];

    do
    {
	unsigned bit = bs.GetData(1);

	pos += (bit == 0 ? data->m_v : data->m_w);
	data = &table.m_data[pos];
    } while (data->m_type == 0);

    samples.push_back(DecodeSign(bs, data->m_v));
    samples.push_back(DecodeSign(bs, data->m_w));
    samples.push_back(DecodeSign(bs, data->m_x));
    samples.push_back(DecodeSign(bs, data->m_y));
}
