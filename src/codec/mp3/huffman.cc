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

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table3[17] =
{
    {0, 1, 14}, {0, 1, 12}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 2, 2}, {1, 0, 2}, {1, 1, 2}, {0, 1, 2},
    {1, 2, 1}, {1, 2, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 2}, {1, 0, 1}, {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table5[31] =
{
    {0, 1, 30}, {0, 1, 26}, {0, 1, 24}, {0, 1, 16}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 3, 3}, {1, 2, 3},
    {1, 3, 2}, {1, 3, 1}, {0, 1, 4}, {0, 1, 2}, {1, 1, 3}, {1, 0, 3}, {0, 1, 2}, {1, 3, 0}, {1, 2, 2}, {0, 1, 4},
    {0, 1, 2}, {1, 1, 2}, {1, 2, 1}, {0, 1, 2}, {1, 0, 2}, {1, 2, 0}, {1, 1, 1}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table6[31] =
{
    {0, 1, 26}, {0, 1, 20}, {0, 1, 14}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 3, 3}, {1, 0, 3}, {1, 2, 3},
    {0, 1, 2}, {1, 3, 2}, {1, 3, 0}, {0, 1, 2}, {1, 1, 3}, {1, 3, 1}, {0, 1, 4}, {0, 1, 2}, {1, 2, 2}, {1, 0, 2},
    {1, 1, 2}, {0, 1, 4}, {0, 1, 2}, {1, 2, 1}, {1, 2, 0}, {1, 0, 1}, {0, 1, 2}, {1, 1, 1}, {0, 1, 2}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table7[71] =
{
    {0, 1, 70}, {0, 1, 66}, {0, 1, 58}, {0, 1, 40}, {0, 1, 30}, {0, 1, 18}, {0, 1, 12}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 5, 5}, {1, 4, 5}, {0, 1, 2}, {1, 5, 4}, {1, 5, 3}, {0, 1, 2}, {1, 3, 5}, {1, 4, 4}, {0, 1, 4}, {0, 1, 2},
    {1, 2, 5}, {1, 5, 2}, {1, 1, 5}, {0, 1, 6}, {0, 1, 2}, {1, 5, 1}, {0, 1, 2}, {1, 0, 5}, {1, 3, 4}, {0, 1, 2},
    {1, 5, 0}, {0, 1, 2}, {1, 4, 3}, {1, 3, 3}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 2, 4}, {1, 4, 2}, {1, 1, 4},
    {0, 1, 2}, {1, 4, 1}, {1, 4, 0}, {0, 1, 12}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 0, 4}, {1, 2, 3}, {0, 1, 2},
    {1, 3, 2}, {1, 0, 3}, {0, 1, 2}, {1, 1, 3}, {1, 3, 1}, {0, 1, 4}, {0, 1, 2}, {1, 3, 0}, {1, 2, 2}, {1, 1, 2},
    {0, 1, 6}, {0, 1, 2}, {1, 2, 1}, {0, 1, 2}, {1, 0, 2}, {1, 2, 0}, {1, 1, 1}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table8[71] =
{
    {0, 1, 66}, {0, 1, 64}, {0, 1, 60}, {0, 1, 46}, {0, 1, 32}, {0, 1, 20}, {0, 1, 14}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4},
    {0, 1, 2}, {1, 5, 5}, {1, 5, 4}, {1, 4, 5}, {1, 5, 3}, {0, 1, 4}, {0, 1, 2}, {1, 3, 5}, {1, 4, 4}, {1, 2, 5},
    {0, 1, 4}, {0, 1, 2}, {1, 5, 2}, {1, 0, 5}, {1, 1, 5}, {0, 1, 6}, {0, 1, 2}, {1, 5, 1}, {0, 1, 2}, {1, 3, 4},
    {1, 4, 3}, {0, 1, 4}, {0, 1, 2}, {1, 5, 0}, {1, 3, 3}, {1, 2, 4}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 4, 2},
    {1, 1, 4}, {1, 4, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 4}, {1, 4, 0}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2}, {0, 1, 10},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 1, 3}, {1, 3, 1}, {0, 1, 2}, {1, 0, 3}, {1, 3, 0}, {1, 2, 2}, {0, 1, 2},
    {1, 0, 2}, {1, 2, 0}, {0, 1, 2}, {1, 1, 2}, {1, 2, 1}, {1, 1, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table9[71] =
{
    {0, 1, 64}, {0, 1, 54}, {0, 1, 42}, {0, 1, 30}, {0, 1, 20}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 5},
    {1, 4, 5}, {1, 3, 5}, {0, 1, 2}, {1, 5, 3}, {0, 1, 2}, {1, 5, 4}, {1, 0, 5}, {0, 1, 4}, {0, 1, 2}, {1, 4, 4},
    {1, 2, 5}, {0, 1, 2}, {1, 5, 2}, {1, 1, 5}, {0, 1, 4}, {0, 1, 2}, {1, 5, 1}, {1, 3, 4}, {0, 1, 2}, {1, 4, 3},
    {0, 1, 2}, {1, 5, 0}, {1, 0, 4}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 2, 4}, {1, 4, 2}, {0, 1, 2}, {1, 3, 3},
    {1, 4, 0}, {0, 1, 2}, {1, 1, 4}, {1, 4, 1}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2}, {1, 1, 3},
    {0, 1, 2}, {1, 3, 1}, {0, 1, 2}, {1, 0, 3}, {1, 3, 0}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 2, 2}, {1, 0, 2},
    {1, 1, 2}, {0, 1, 2}, {1, 2, 1}, {1, 2, 0}, {0, 1, 4}, {0, 1, 2}, {1, 1, 1}, {1, 0, 1}, {0, 1, 2}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table10[127] =
{
    {0, 1, 126}, {0, 1, 122}, {0, 1, 112}, {0, 1, 84}, {0, 1, 56}, {0, 1, 36}, {0, 1, 22}, {0, 1, 14}, {0, 1, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 7, 7}, {1, 6, 7}, {0, 1, 2}, {1, 7, 6}, {1, 5, 7}, {0, 1, 4}, {0, 1, 2}, {1, 7, 5}, {1, 6, 6},
    {1, 4, 7}, {0, 1, 4}, {0, 1, 2}, {1, 7, 4}, {1, 5, 6}, {0, 1, 2}, {1, 6, 5}, {1, 3, 7}, {0, 1, 10}, {0, 1, 4},
    {0, 1, 2}, {1, 7, 3}, {1, 4, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 5}, {1, 5, 4}, {1, 6, 3}, {0, 1, 2}, {1, 2, 7},
    {1, 7, 2}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 6, 4}, {1, 0, 7}, {1, 7, 0}, {0, 1, 2}, {1, 6, 2},
    {0, 1, 2}, {1, 4, 5}, {1, 3, 5}, {0, 1, 6}, {0, 1, 2}, {1, 0, 6}, {0, 1, 2}, {1, 5, 3}, {1, 4, 4}, {1, 1, 7},
    {0, 1, 18}, {0, 1, 6}, {0, 1, 2}, {1, 7, 1}, {0, 1, 2}, {1, 3, 6}, {1, 2, 6}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2},
    {1, 2, 5}, {1, 5, 2}, {1, 1, 5}, {0, 1, 2}, {1, 5, 1}, {0, 1, 2}, {1, 3, 4}, {1, 4, 3}, {0, 1, 4}, {0, 1, 2},
    {1, 1, 6}, {1, 6, 1}, {0, 1, 2}, {1, 6, 0}, {0, 1, 2}, {1, 0, 5}, {1, 5, 0}, {0, 1, 20}, {0, 1, 12}, {0, 1, 8},
    {0, 1, 4}, {0, 1, 2}, {1, 2, 4}, {1, 4, 2}, {0, 1, 2}, {1, 3, 3}, {1, 0, 4}, {0, 1, 2}, {1, 1, 4}, {1, 4, 1},
    {0, 1, 4}, {0, 1, 2}, {1, 4, 0}, {1, 2, 3}, {0, 1, 2}, {1, 3, 2}, {1, 0, 3}, {0, 1, 4}, {0, 1, 2}, {1, 1, 3},
    {1, 3, 1}, {0, 1, 2}, {1, 3, 0}, {1, 2, 2}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 1, 2}, {1, 2, 1}, {0, 1, 2},
    {1, 0, 2}, {1, 2, 0}, {1, 1, 1}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0}, {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table11[127] =
{
    {0, 1, 122}, {0, 1, 114}, {0, 1, 90}, {0, 1, 60}, {0, 1, 44}, {0, 1, 28}, {0, 1, 18}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 7, 7}, {1, 6, 7}, {0, 1, 2}, {1, 7, 6}, {1, 7, 5}, {0, 1, 4}, {0, 1, 2}, {1, 6, 6}, {1, 4, 7}, {0, 1, 2},
    {1, 7, 4}, {0, 1, 2}, {1, 5, 7}, {1, 5, 5}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 6}, {1, 6, 5}, {1, 3, 7},
    {0, 1, 2}, {1, 7, 3}, {1, 4, 6}, {0, 1, 10}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 4, 5}, {1, 5, 4}, {0, 1, 2},
    {1, 3, 5}, {1, 5, 3}, {1, 2, 7}, {0, 1, 2}, {1, 7, 2}, {0, 1, 2}, {1, 6, 4}, {1, 0, 7}, {0, 1, 6}, {0, 1, 2},
    {1, 7, 1}, {0, 1, 2}, {1, 1, 7}, {1, 7, 0}, {0, 1, 4}, {0, 1, 2}, {1, 3, 6}, {1, 6, 3}, {0, 1, 2}, {1, 6, 0},
    {0, 1, 2}, {1, 4, 4}, {1, 2, 5}, {0, 1, 14}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 2}, {1, 0, 5},
    {1, 1, 5}, {1, 6, 2}, {0, 1, 4}, {0, 1, 2}, {1, 2, 6}, {1, 0, 6}, {1, 1, 6}, {0, 1, 6}, {0, 1, 2}, {1, 6, 1},
    {0, 1, 2}, {1, 5, 1}, {1, 3, 4}, {0, 1, 6}, {0, 1, 2}, {1, 5, 0}, {0, 1, 2}, {1, 4, 3}, {1, 3, 3}, {0, 1, 2},
    {1, 2, 4}, {1, 4, 2}, {0, 1, 16}, {0, 1, 12}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 1, 4}, {1, 4, 1}, {0, 1, 2},
    {1, 0, 4}, {1, 4, 0}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2}, {0, 1, 2}, {1, 1, 3}, {1, 3, 1}, {0, 1, 6}, {0, 1, 4},
    {0, 1, 2}, {1, 0, 3}, {1, 3, 0}, {1, 2, 2}, {1, 2, 1}, {0, 1, 6}, {0, 1, 2}, {1, 1, 2}, {0, 1, 2}, {1, 0, 2},
    {1, 2, 0}, {1, 1, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0}, {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table12[127] =
{
    {0, 1, 116}, {0, 1, 100}, {0, 1, 74}, {0, 1, 46}, {0, 1, 28}, {0, 1, 18}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2},
    {1, 7, 7}, {1, 6, 7}, {1, 7, 6}, {0, 1, 2}, {1, 5, 7}, {1, 7, 5}, {0, 1, 4}, {0, 1, 2}, {1, 6, 6}, {1, 4, 7},
    {0, 1, 2}, {1, 7, 4}, {1, 6, 5}, {0, 1, 4}, {0, 1, 2}, {1, 5, 6}, {1, 3, 7}, {0, 1, 4}, {0, 1, 2}, {1, 7, 3},
    {1, 5, 5}, {1, 2, 7}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 7, 2}, {1, 4, 6}, {0, 1, 2}, {1, 6, 4}, {1, 1, 7},
    {0, 1, 6}, {0, 1, 2}, {1, 7, 1}, {0, 1, 2}, {1, 0, 7}, {1, 7, 0}, {0, 1, 2}, {1, 3, 6}, {1, 6, 3}, {0, 1, 14},
    {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 4, 5}, {1, 5, 4}, {0, 1, 2}, {1, 4, 4}, {0, 1, 2}, {1, 0, 6}, {1, 0, 5},
    {0, 1, 2}, {1, 2, 6}, {1, 6, 2}, {0, 1, 6}, {0, 1, 2}, {1, 6, 1}, {0, 1, 2}, {1, 1, 6}, {1, 6, 0}, {0, 1, 4},
    {0, 1, 2}, {1, 3, 5}, {1, 5, 3}, {0, 1, 2}, {1, 2, 5}, {1, 5, 2}, {0, 1, 18}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 1, 5}, {1, 5, 1}, {0, 1, 2}, {1, 3, 4}, {1, 4, 3}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 0}, {1, 0, 4},
    {1, 2, 4}, {0, 1, 2}, {1, 4, 2}, {1, 1, 4}, {0, 1, 4}, {0, 1, 2}, {1, 3, 3}, {1, 4, 1}, {0, 1, 2}, {1, 2, 3},
    {1, 3, 2}, {0, 1, 12}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 4, 0}, {1, 0, 3}, {1, 3, 0}, {1, 1, 3},
    {0, 1, 2}, {1, 3, 1}, {1, 2, 2}, {0, 1, 2}, {1, 1, 2}, {1, 2, 1}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2},
    {1, 0, 2}, {1, 2, 0}, {1, 0, 0}, {1, 1, 1}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table13[511] =
{
    {0, 1, 510}, {0, 1, 504}, {0, 1, 476}, {0, 1, 406}, {0, 1, 334}, {0, 1, 266}, {0, 1, 206}, {0, 1, 154}, {0, 1, 116}, {0, 1, 84},
    {0, 1, 54}, {0, 1, 36}, {0, 1, 22}, {0, 1, 14}, {0, 1, 10}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 15, 14},
    {1, 15, 12}, {1, 15, 13}, {1, 14, 13}, {1, 15, 15}, {0, 1, 2}, {1, 14, 15}, {1, 13, 15}, {0, 1, 4}, {0, 1, 2}, {1, 14, 14},
    {1, 12, 15}, {0, 1, 2}, {1, 13, 14}, {1, 11, 15}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 15, 11}, {1, 12, 14}, {0, 1, 2},
    {1, 13, 12}, {0, 1, 2}, {1, 10, 15}, {1, 14, 9}, {0, 1, 2}, {1, 14, 12}, {1, 13, 13}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4},
    {0, 1, 2}, {1, 15, 10}, {1, 12, 13}, {1, 11, 14}, {0, 1, 2}, {1, 14, 11}, {1, 9, 15}, {0, 1, 4}, {0, 1, 2}, {1, 15, 9},
    {1, 14, 10}, {0, 1, 2}, {1, 11, 13}, {1, 13, 11}, {0, 1, 18}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 8, 15}, {1, 15, 8},
    {0, 1, 2}, {1, 12, 12}, {0, 1, 2}, {1, 10, 14}, {1, 9, 14}, {0, 1, 6}, {0, 1, 2}, {1, 8, 14}, {0, 1, 2}, {1, 7, 15},
    {1, 7, 14}, {1, 15, 7}, {0, 1, 6}, {0, 1, 2}, {1, 13, 10}, {0, 1, 2}, {1, 10, 13}, {1, 11, 12}, {0, 1, 4}, {0, 1, 2},
    {1, 12, 11}, {1, 15, 6}, {1, 6, 15}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 14, 8}, {1, 5, 15}, {0, 1, 2},
    {1, 9, 13}, {1, 13, 9}, {0, 1, 4}, {0, 1, 2}, {1, 15, 5}, {1, 14, 7}, {0, 1, 2}, {1, 10, 12}, {1, 11, 11}, {0, 1, 10},
    {0, 1, 4}, {0, 1, 2}, {1, 4, 15}, {1, 15, 4}, {0, 1, 4}, {0, 1, 2}, {1, 12, 10}, {1, 14, 6}, {1, 15, 3}, {0, 1, 2},
    {1, 3, 15}, {0, 1, 2}, {1, 8, 13}, {1, 13, 8}, {0, 1, 22}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 2, 15}, {1, 15, 2},
    {0, 1, 4}, {0, 1, 2}, {1, 6, 14}, {1, 9, 12}, {1, 0, 15}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 12, 9}, {1, 5, 14},
    {1, 10, 11}, {0, 1, 4}, {0, 1, 2}, {1, 7, 13}, {1, 13, 7}, {1, 4, 14}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2},
    {1, 12, 8}, {1, 13, 6}, {1, 3, 14}, {0, 1, 2}, {1, 11, 9}, {0, 1, 2}, {1, 9, 11}, {1, 10, 10}, {0, 1, 2}, {1, 1, 15},
    {1, 15, 1}, {0, 1, 24}, {0, 1, 14}, {0, 1, 6}, {0, 1, 2}, {1, 15, 0}, {0, 1, 2}, {1, 11, 10}, {1, 14, 5}, {0, 1, 4},
    {0, 1, 2}, {1, 14, 4}, {1, 8, 12}, {0, 1, 2}, {1, 6, 13}, {1, 14, 3}, {0, 1, 6}, {0, 1, 2}, {1, 14, 2}, {0, 1, 2},
    {1, 2, 14}, {1, 0, 14}, {0, 1, 2}, {1, 1, 14}, {1, 14, 1}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 14, 0},
    {1, 5, 13}, {0, 1, 2}, {1, 13, 5}, {1, 7, 12}, {0, 1, 4}, {0, 1, 2}, {1, 12, 7}, {1, 4, 13}, {0, 1, 2}, {1, 8, 11},
    {1, 11, 8}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 13, 4}, {1, 9, 10}, {0, 1, 2}, {1, 10, 9}, {1, 6, 12}, {0, 1, 2},
    {1, 12, 6}, {1, 3, 13}, {0, 1, 38}, {0, 1, 22}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 13, 3}, {1, 7, 11},
    {1, 2, 13}, {0, 1, 2}, {1, 13, 2}, {1, 1, 13}, {0, 1, 6}, {0, 1, 2}, {1, 11, 7}, {0, 1, 2}, {1, 5, 12}, {1, 12, 5},
    {0, 1, 4}, {0, 1, 2}, {1, 9, 9}, {1, 7, 10}, {1, 12, 3}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 10, 7},
    {1, 9, 7}, {1, 4, 11}, {1, 13, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 13}, {1, 13, 0}, {0, 1, 2}, {1, 8, 10}, {1, 10, 8},
    {0, 1, 12}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 4, 12}, {1, 12, 4}, {0, 1, 2}, {1, 6, 11}, {1, 11, 6}, {0, 1, 2},
    {1, 3, 12}, {1, 2, 12}, {0, 1, 4}, {0, 1, 2}, {1, 12, 2}, {1, 5, 11}, {0, 1, 4}, {0, 1, 2}, {1, 11, 5}, {1, 8, 9},
    {1, 1, 12}, {0, 1, 44}, {0, 1, 24}, {0, 1, 12}, {0, 1, 6}, {0, 1, 2}, {1, 12, 1}, {0, 1, 2}, {1, 9, 8}, {1, 0, 12},
    {0, 1, 2}, {1, 12, 0}, {0, 1, 2}, {1, 11, 4}, {1, 6, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 10, 6}, {1, 7, 9},
    {1, 3, 11}, {0, 1, 2}, {1, 11, 3}, {0, 1, 2}, {1, 8, 8}, {1, 5, 10}, {0, 1, 12}, {0, 1, 6}, {0, 1, 2}, {1, 2, 11},
    {0, 1, 2}, {1, 10, 5}, {1, 6, 9}, {0, 1, 2}, {1, 10, 4}, {0, 1, 2}, {1, 7, 8}, {1, 8, 7}, {0, 1, 6}, {0, 1, 2},
    {1, 9, 4}, {0, 1, 2}, {1, 7, 7}, {1, 7, 6}, {1, 11, 2}, {0, 1, 12}, {0, 1, 4}, {0, 1, 2}, {1, 1, 11}, {1, 11, 1},
    {0, 1, 4}, {0, 1, 2}, {1, 0, 11}, {1, 11, 0}, {0, 1, 2}, {1, 9, 6}, {1, 4, 10}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 3, 10}, {1, 10, 3}, {0, 1, 2}, {1, 5, 9}, {1, 9, 5}, {0, 1, 2}, {1, 2, 10}, {1, 10, 2}, {0, 1, 48}, {0, 1, 24},
    {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 1, 10}, {1, 10, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 10}, {1, 6, 8}, {1, 10, 0},
    {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 8, 6}, {1, 4, 9}, {1, 9, 3}, {0, 1, 4}, {0, 1, 2}, {1, 3, 9}, {1, 5, 8},
    {0, 1, 2}, {1, 8, 5}, {1, 6, 7}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 2, 9}, {1, 9, 2}, {0, 1, 4}, {0, 1, 2},
    {1, 5, 7}, {1, 7, 5}, {1, 3, 8}, {0, 1, 6}, {0, 1, 2}, {1, 8, 3}, {0, 1, 2}, {1, 6, 6}, {1, 4, 7}, {0, 1, 4},
    {0, 1, 2}, {1, 7, 4}, {1, 5, 6}, {0, 1, 2}, {1, 6, 5}, {1, 7, 3}, {0, 1, 12}, {0, 1, 4}, {0, 1, 2}, {1, 1, 9},
    {1, 9, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 9}, {1, 9, 0}, {0, 1, 2}, {1, 4, 8}, {1, 8, 4}, {0, 1, 8}, {0, 1, 6},
    {0, 1, 2}, {1, 7, 2}, {0, 1, 2}, {1, 4, 6}, {1, 6, 4}, {1, 2, 8}, {0, 1, 2}, {1, 8, 2}, {1, 1, 8}, {0, 1, 42},
    {0, 1, 28}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 3, 7}, {1, 2, 7}, {1, 1, 7}, {0, 1, 2}, {1, 7, 1},
    {0, 1, 2}, {1, 5, 5}, {1, 0, 7}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 7, 0}, {1, 3, 6}, {0, 1, 2}, {1, 6, 3},
    {1, 4, 5}, {0, 1, 4}, {0, 1, 2}, {1, 5, 4}, {1, 2, 6}, {0, 1, 2}, {1, 6, 2}, {1, 3, 5}, {0, 1, 6}, {0, 1, 2},
    {1, 8, 1}, {0, 1, 2}, {1, 0, 8}, {1, 8, 0}, {0, 1, 4}, {0, 1, 2}, {1, 1, 6}, {1, 6, 1}, {0, 1, 2}, {1, 0, 6},
    {1, 6, 0}, {0, 1, 14}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 3}, {1, 4, 4}, {1, 2, 5}, {0, 1, 2},
    {1, 5, 2}, {1, 0, 5}, {0, 1, 2}, {1, 1, 5}, {1, 5, 1}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 3, 4}, {1, 4, 3},
    {0, 1, 2}, {1, 5, 0}, {1, 2, 4}, {0, 1, 4}, {0, 1, 2}, {1, 4, 2}, {1, 3, 3}, {1, 1, 4}, {0, 1, 20}, {0, 1, 12},
    {0, 1, 6}, {0, 1, 2}, {1, 4, 1}, {0, 1, 2}, {1, 0, 4}, {1, 4, 0}, {0, 1, 4}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2},
    {1, 1, 3}, {0, 1, 4}, {0, 1, 2}, {1, 3, 1}, {1, 0, 3}, {0, 1, 2}, {1, 3, 0}, {1, 2, 2}, {0, 1, 4}, {0, 1, 2},
    {1, 1, 2}, {1, 2, 1}, {0, 1, 2}, {1, 0, 2}, {1, 2, 0}, {0, 1, 4}, {0, 1, 2}, {1, 1, 1}, {1, 0, 1}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table15[511] =
{
    {0, 1, 496}, {0, 1, 446}, {0, 1, 356}, {0, 1, 264}, {0, 1, 184}, {0, 1, 116}, {0, 1, 78}, {0, 1, 44}, {0, 1, 28}, {0, 1, 14},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 15, 15}, {1, 14, 15}, {0, 1, 2}, {1, 15, 14}, {1, 13, 15}, {0, 1, 2}, {1, 14, 14},
    {0, 1, 2}, {1, 15, 13}, {1, 12, 15}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 15, 12}, {1, 13, 14}, {0, 1, 2}, {1, 14, 13},
    {1, 11, 15}, {0, 1, 2}, {1, 15, 11}, {0, 1, 2}, {1, 12, 14}, {1, 14, 12}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 13, 13},
    {1, 10, 15}, {0, 1, 2}, {1, 15, 10}, {1, 11, 14}, {0, 1, 4}, {0, 1, 2}, {1, 14, 11}, {1, 12, 13}, {0, 1, 2}, {1, 13, 12},
    {1, 9, 15}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 15, 9}, {1, 14, 10}, {0, 1, 2}, {1, 11, 13}, {1, 13, 11},
    {0, 1, 4}, {0, 1, 2}, {1, 8, 15}, {1, 15, 8}, {0, 1, 2}, {1, 12, 12}, {1, 9, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 14, 9}, {1, 7, 15}, {0, 1, 2}, {1, 15, 7}, {1, 10, 13}, {0, 1, 4}, {0, 1, 2}, {1, 13, 10}, {1, 11, 12}, {0, 1, 2},
    {1, 6, 15}, {0, 1, 2}, {1, 10, 14}, {1, 0, 15}, {0, 1, 20}, {0, 1, 12}, {0, 1, 4}, {0, 1, 2}, {1, 12, 11}, {1, 15, 6},
    {0, 1, 4}, {0, 1, 2}, {1, 8, 14}, {1, 14, 8}, {0, 1, 2}, {1, 5, 15}, {1, 9, 13}, {0, 1, 4}, {0, 1, 2}, {1, 15, 5},
    {1, 7, 14}, {0, 1, 2}, {1, 14, 7}, {1, 10, 12}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 12, 10}, {1, 11, 11}, {0, 1, 4},
    {0, 1, 2}, {1, 13, 9}, {1, 8, 13}, {1, 4, 15}, {0, 1, 4}, {0, 1, 2}, {1, 15, 4}, {1, 3, 15}, {0, 1, 2}, {1, 15, 3},
    {1, 13, 8}, {0, 1, 34}, {0, 1, 18}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 14, 6}, {1, 2, 15}, {0, 1, 2}, {1, 15, 2},
    {0, 1, 2}, {1, 6, 14}, {1, 15, 0}, {0, 1, 4}, {0, 1, 2}, {1, 1, 15}, {1, 15, 1}, {0, 1, 2}, {1, 9, 12}, {1, 12, 9},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 5, 14}, {1, 10, 11}, {0, 1, 2}, {1, 11, 10}, {1, 14, 5}, {0, 1, 4}, {0, 1, 2},
    {1, 7, 13}, {1, 13, 7}, {0, 1, 2}, {1, 4, 14}, {1, 14, 4}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 8, 12},
    {1, 12, 8}, {0, 1, 2}, {1, 3, 14}, {1, 6, 13}, {0, 1, 4}, {0, 1, 2}, {1, 13, 6}, {1, 14, 3}, {0, 1, 2}, {1, 9, 11},
    {1, 11, 9}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 2, 14}, {1, 10, 10}, {0, 1, 2}, {1, 14, 2}, {1, 1, 14}, {0, 1, 6},
    {0, 1, 2}, {1, 14, 1}, {0, 1, 2}, {1, 0, 14}, {1, 14, 0}, {0, 1, 2}, {1, 5, 13}, {1, 13, 5}, {0, 1, 46}, {0, 1, 26},
    {0, 1, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 7, 12}, {1, 12, 7}, {0, 1, 2}, {1, 4, 13}, {1, 8, 11}, {0, 1, 2},
    {1, 13, 4}, {0, 1, 2}, {1, 11, 8}, {1, 9, 10}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 10, 9}, {1, 6, 12}, {0, 1, 2},
    {1, 12, 6}, {1, 3, 13}, {0, 1, 2}, {1, 13, 3}, {1, 13, 2}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 2, 13},
    {1, 0, 13}, {1, 1, 13}, {0, 1, 2}, {1, 7, 11}, {1, 11, 7}, {0, 1, 6}, {0, 1, 2}, {1, 13, 1}, {0, 1, 2}, {1, 5, 12},
    {1, 13, 0}, {0, 1, 2}, {1, 12, 5}, {1, 8, 10}, {0, 1, 18}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 10, 8}, {1, 4, 12},
    {0, 1, 2}, {1, 12, 4}, {1, 6, 11}, {0, 1, 6}, {0, 1, 2}, {1, 11, 6}, {0, 1, 2}, {1, 9, 9}, {1, 0, 12}, {0, 1, 2},
    {1, 3, 12}, {1, 12, 3}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 7, 10}, {1, 10, 7}, {0, 1, 2}, {1, 10, 6}, {0, 1, 2},
    {1, 12, 0}, {1, 0, 11}, {0, 1, 2}, {1, 12, 2}, {0, 1, 2}, {1, 2, 12}, {1, 5, 11}, {0, 1, 56}, {0, 1, 30}, {0, 1, 16},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 11, 5}, {1, 1, 12}, {0, 1, 2}, {1, 8, 9}, {1, 9, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 12, 1}, {1, 4, 11}, {0, 1, 2}, {1, 11, 4}, {1, 6, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 3, 11}, {1, 7, 9},
    {1, 11, 3}, {0, 1, 4}, {0, 1, 2}, {1, 9, 7}, {1, 8, 8}, {0, 1, 2}, {1, 2, 11}, {1, 5, 10}, {0, 1, 12}, {0, 1, 6},
    {0, 1, 2}, {1, 11, 2}, {0, 1, 2}, {1, 10, 5}, {1, 1, 11}, {0, 1, 2}, {1, 11, 1}, {0, 1, 2}, {1, 11, 0}, {1, 6, 9},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 9, 6}, {1, 4, 10}, {0, 1, 2}, {1, 10, 4}, {1, 7, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 8, 7}, {1, 3, 10}, {1, 10, 3}, {0, 1, 18}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 5, 9}, {1, 9, 5}, {0, 1, 2},
    {1, 2, 10}, {1, 10, 2}, {0, 1, 4}, {0, 1, 2}, {1, 1, 10}, {1, 10, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 10}, {1, 10, 0},
    {1, 6, 8}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 8, 6}, {1, 4, 9}, {0, 1, 2}, {1, 9, 4}, {1, 3, 9}, {0, 1, 6},
    {0, 1, 2}, {1, 9, 3}, {0, 1, 2}, {1, 7, 7}, {1, 0, 9}, {0, 1, 2}, {1, 5, 8}, {1, 8, 5}, {0, 1, 54}, {0, 1, 30},
    {0, 1, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 2, 9}, {1, 6, 7}, {0, 1, 2}, {1, 7, 6}, {1, 9, 2}, {0, 1, 2},
    {1, 9, 1}, {0, 1, 2}, {1, 1, 9}, {1, 9, 0}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 4, 8}, {1, 8, 4}, {0, 1, 2},
    {1, 5, 7}, {1, 7, 5}, {0, 1, 4}, {0, 1, 2}, {1, 3, 8}, {1, 8, 3}, {0, 1, 2}, {1, 6, 6}, {1, 4, 7}, {0, 1, 8},
    {0, 1, 4}, {0, 1, 2}, {1, 2, 8}, {1, 8, 2}, {0, 1, 2}, {1, 1, 8}, {1, 8, 1}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 7, 4}, {1, 0, 8}, {0, 1, 2}, {1, 8, 0}, {1, 5, 6}, {0, 1, 4}, {0, 1, 2}, {1, 6, 5}, {1, 3, 7}, {0, 1, 2},
    {1, 7, 3}, {1, 4, 6}, {0, 1, 18}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 2, 7}, {1, 7, 2}, {0, 1, 2}, {1, 6, 4},
    {1, 1, 7}, {0, 1, 4}, {0, 1, 2}, {1, 5, 5}, {1, 7, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 7}, {1, 7, 0}, {1, 3, 6},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 6, 3}, {1, 4, 5}, {0, 1, 2}, {1, 5, 4}, {1, 2, 6}, {0, 1, 4}, {0, 1, 2},
    {1, 6, 2}, {1, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 0, 6}, {1, 6, 0}, {1, 3, 5}, {0, 1, 34}, {0, 1, 20}, {0, 1, 10},
    {0, 1, 6}, {0, 1, 2}, {1, 6, 1}, {0, 1, 2}, {1, 5, 3}, {1, 4, 4}, {0, 1, 2}, {1, 2, 5}, {1, 5, 2}, {0, 1, 4},
    {0, 1, 2}, {1, 1, 5}, {1, 5, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 5}, {1, 5, 0}, {1, 3, 4}, {0, 1, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 4, 3}, {1, 2, 4}, {0, 1, 2}, {1, 4, 2}, {1, 3, 3}, {0, 1, 2}, {1, 4, 1}, {0, 1, 2}, {1, 1, 4},
    {1, 0, 4}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2}, {0, 1, 4}, {0, 1, 2}, {1, 4, 0}, {1, 0, 3},
    {1, 1, 3}, {0, 1, 4}, {0, 1, 2}, {1, 3, 1}, {1, 3, 0}, {1, 2, 2}, {0, 1, 10}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 1, 2}, {1, 2, 1}, {0, 1, 2}, {1, 0, 2}, {1, 2, 0}, {1, 1, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 1}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table16[511] =
{
    {0, 1, 510}, {0, 1, 504}, {0, 1, 462}, {0, 1, 324}, {0, 1, 104}, {0, 1, 38}, {0, 1, 28}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 14, 15}, {1, 15, 14}, {0, 1, 2}, {1, 13, 15}, {1, 15, 13}, {0, 1, 4}, {0, 1, 2}, {1, 12, 15}, {1, 15, 12},
    {0, 1, 2}, {1, 11, 15}, {1, 15, 11}, {0, 1, 6}, {0, 1, 2}, {1, 10, 15}, {0, 1, 2}, {1, 15, 10}, {1, 9, 15}, {0, 1, 4},
    {0, 1, 2}, {1, 15, 9}, {1, 15, 8}, {1, 8, 15}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 7, 15}, {1, 15, 7}, {0, 1, 2},
    {1, 6, 15}, {1, 15, 6}, {1, 15, 15}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 15}, {1, 15, 5}, {1, 4, 15},
    {0, 1, 2}, {1, 15, 4}, {1, 15, 3}, {0, 1, 54}, {0, 1, 2}, {1, 15, 0}, {0, 1, 2}, {1, 3, 15}, {0, 1, 30}, {0, 1, 20},
    {0, 1, 14}, {0, 1, 8}, {0, 1, 6}, {0, 1, 2}, {1, 12, 14}, {0, 1, 2}, {1, 14, 12}, {1, 13, 13}, {1, 13, 14}, {0, 1, 2},
    {1, 14, 9}, {0, 1, 2}, {1, 14, 10}, {1, 13, 9}, {0, 1, 2}, {1, 14, 14}, {0, 1, 2}, {1, 14, 13}, {1, 14, 11}, {0, 1, 4},
    {0, 1, 2}, {1, 11, 14}, {1, 12, 13}, {0, 1, 4}, {0, 1, 2}, {1, 13, 12}, {1, 13, 11}, {1, 10, 14}, {0, 1, 12}, {0, 1, 6},
    {0, 1, 2}, {1, 12, 12}, {0, 1, 2}, {1, 10, 13}, {1, 13, 10}, {0, 1, 4}, {0, 1, 2}, {1, 7, 14}, {1, 10, 12}, {1, 12, 10},
    {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 12, 9}, {1, 7, 13}, {1, 5, 14}, {1, 11, 13}, {1, 15, 2}, {0, 1, 94}, {0, 1, 6},
    {0, 1, 4}, {0, 1, 2}, {1, 2, 15}, {1, 0, 15}, {1, 1, 15}, {0, 1, 2}, {1, 15, 1}, {0, 1, 50}, {0, 1, 26}, {0, 1, 14},
    {0, 1, 6}, {0, 1, 2}, {1, 9, 14}, {0, 1, 2}, {1, 11, 12}, {1, 12, 11}, {0, 1, 4}, {0, 1, 2}, {1, 8, 14}, {1, 14, 8},
    {0, 1, 2}, {1, 9, 13}, {1, 14, 7}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 11, 11}, {1, 8, 13}, {0, 1, 2}, {1, 13, 8},
    {1, 6, 14}, {0, 1, 2}, {1, 14, 6}, {1, 9, 12}, {0, 1, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 10, 11}, {1, 11, 10},
    {0, 1, 2}, {1, 14, 5}, {1, 13, 7}, {0, 1, 2}, {1, 4, 14}, {0, 1, 2}, {1, 14, 4}, {1, 8, 12}, {0, 1, 4}, {0, 1, 2},
    {1, 12, 8}, {1, 3, 14}, {0, 1, 2}, {1, 6, 13}, {0, 1, 2}, {1, 13, 6}, {1, 9, 11}, {0, 1, 20}, {0, 1, 12}, {0, 1, 6},
    {0, 1, 4}, {0, 1, 2}, {1, 11, 9}, {1, 10, 10}, {1, 14, 1}, {0, 1, 2}, {1, 13, 4}, {0, 1, 2}, {1, 11, 8}, {1, 10, 9},
    {0, 1, 6}, {0, 1, 2}, {1, 7, 11}, {0, 1, 2}, {1, 11, 7}, {1, 13, 0}, {1, 14, 3}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 0, 14}, {1, 14, 0}, {0, 1, 2}, {1, 5, 13}, {1, 13, 5}, {0, 1, 4}, {0, 1, 2}, {1, 7, 12}, {1, 12, 7}, {0, 1, 2},
    {1, 4, 13}, {1, 8, 11}, {0, 1, 76}, {0, 1, 46}, {0, 1, 28}, {0, 1, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 9, 10},
    {1, 6, 12}, {0, 1, 2}, {1, 12, 6}, {1, 3, 13}, {0, 1, 4}, {0, 1, 2}, {1, 5, 12}, {1, 12, 5}, {1, 0, 13}, {0, 1, 8},
    {0, 1, 4}, {0, 1, 2}, {1, 8, 10}, {1, 10, 8}, {0, 1, 2}, {1, 9, 9}, {1, 4, 12}, {0, 1, 4}, {0, 1, 2}, {1, 11, 6},
    {1, 7, 10}, {1, 3, 12}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 11}, {1, 8, 9}, {1, 1, 12}, {0, 1, 2},
    {1, 12, 0}, {0, 1, 2}, {1, 9, 8}, {1, 7, 9}, {0, 1, 2}, {1, 14, 2}, {0, 1, 2}, {1, 2, 14}, {1, 1, 14}, {0, 1, 16},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 13, 3}, {1, 2, 13}, {0, 1, 2}, {1, 13, 2}, {1, 13, 1}, {0, 1, 6}, {0, 1, 2},
    {1, 3, 11}, {0, 1, 2}, {1, 9, 7}, {1, 8, 8}, {1, 1, 13}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 12, 4}, {1, 6, 11},
    {0, 1, 2}, {1, 12, 3}, {1, 10, 7}, {0, 1, 2}, {1, 2, 12}, {0, 1, 2}, {1, 12, 2}, {1, 11, 5}, {0, 1, 24}, {0, 1, 14},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 12, 1}, {1, 0, 12}, {0, 1, 2}, {1, 4, 11}, {1, 11, 4}, {0, 1, 4}, {0, 1, 2},
    {1, 6, 10}, {1, 10, 6}, {1, 11, 3}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 5, 10}, {1, 10, 5}, {1, 2, 11}, {0, 1, 2},
    {1, 11, 2}, {1, 1, 11}, {0, 1, 14}, {0, 1, 6}, {0, 1, 2}, {1, 11, 1}, {0, 1, 2}, {1, 0, 11}, {1, 11, 0}, {0, 1, 4},
    {0, 1, 2}, {1, 6, 9}, {1, 9, 6}, {0, 1, 2}, {1, 4, 10}, {1, 10, 4}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 7, 8},
    {1, 8, 7}, {1, 10, 3}, {0, 1, 4}, {0, 1, 2}, {1, 3, 10}, {1, 5, 9}, {1, 2, 10}, {0, 1, 98}, {0, 1, 58}, {0, 1, 34},
    {0, 1, 20}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 9, 5}, {1, 6, 8}, {1, 10, 1}, {0, 1, 4}, {0, 1, 2},
    {1, 8, 6}, {1, 7, 7}, {1, 9, 4}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 4, 9}, {1, 5, 7}, {1, 6, 7}, {1, 10, 2},
    {0, 1, 6}, {0, 1, 2}, {1, 1, 10}, {0, 1, 2}, {1, 0, 10}, {1, 10, 0}, {0, 1, 4}, {0, 1, 2}, {1, 3, 9}, {1, 9, 3},
    {0, 1, 2}, {1, 5, 8}, {1, 8, 5}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 2, 9}, {1, 9, 2}, {0, 1, 4}, {0, 1, 2},
    {1, 7, 6}, {1, 0, 9}, {1, 1, 9}, {0, 1, 6}, {0, 1, 2}, {1, 9, 1}, {0, 1, 2}, {1, 9, 0}, {1, 4, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 8, 4}, {1, 7, 5}, {0, 1, 2}, {1, 3, 8}, {1, 8, 3}, {0, 1, 22}, {0, 1, 12}, {0, 1, 6}, {0, 1, 4},
    {0, 1, 2}, {1, 6, 6}, {1, 2, 8}, {1, 8, 2}, {0, 1, 4}, {0, 1, 2}, {1, 4, 7}, {1, 7, 4}, {1, 1, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 8, 1}, {1, 8, 0}, {0, 1, 4}, {0, 1, 2}, {1, 0, 8}, {1, 5, 6}, {1, 3, 7}, {0, 1, 10}, {0, 1, 6},
    {0, 1, 2}, {1, 7, 3}, {0, 1, 2}, {1, 6, 5}, {1, 4, 6}, {0, 1, 2}, {1, 2, 7}, {1, 7, 2}, {0, 1, 6}, {0, 1, 4},
    {0, 1, 2}, {1, 6, 4}, {1, 5, 5}, {1, 0, 7}, {1, 1, 7}, {0, 1, 24}, {0, 1, 14}, {0, 1, 6}, {0, 1, 2}, {1, 7, 1},
    {0, 1, 2}, {1, 7, 0}, {1, 3, 6}, {0, 1, 4}, {0, 1, 2}, {1, 6, 3}, {1, 4, 5}, {0, 1, 2}, {1, 5, 4}, {1, 2, 6},
    {0, 1, 4}, {0, 1, 2}, {1, 6, 2}, {1, 1, 6}, {0, 1, 2}, {1, 6, 1}, {0, 1, 2}, {1, 0, 6}, {1, 6, 0}, {0, 1, 10},
    {0, 1, 6}, {0, 1, 2}, {1, 5, 3}, {0, 1, 2}, {1, 3, 5}, {1, 4, 4}, {0, 1, 2}, {1, 2, 5}, {1, 5, 2}, {0, 1, 2},
    {1, 5, 1}, {0, 1, 2}, {1, 1, 5}, {1, 0, 5}, {0, 1, 34}, {0, 1, 24}, {0, 1, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 3, 4}, {1, 4, 3}, {0, 1, 2}, {1, 5, 0}, {1, 2, 4}, {0, 1, 4}, {0, 1, 2}, {1, 4, 2}, {1, 3, 3}, {1, 1, 4},
    {0, 1, 6}, {0, 1, 2}, {1, 4, 1}, {0, 1, 2}, {1, 0, 4}, {1, 4, 0}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2}, {0, 1, 4},
    {0, 1, 2}, {1, 1, 3}, {1, 3, 1}, {0, 1, 4}, {0, 1, 2}, {1, 0, 3}, {1, 3, 0}, {1, 2, 2}, {0, 1, 4}, {0, 1, 2},
    {1, 1, 2}, {1, 2, 1}, {0, 1, 2}, {1, 0, 2}, {1, 2, 0}, {0, 1, 4}, {0, 1, 2}, {1, 1, 1}, {1, 0, 1}, {1, 1, 0},
    {1, 0, 0}
};

const HuffmanDecoder::HuffmanData HuffmanDecoder::s_table24[511] =
{
    {0, 1, 452}, {0, 1, 118}, {0, 1, 44}, {0, 1, 26}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 14, 15}, {1, 15, 14},
    {0, 1, 2}, {1, 13, 15}, {1, 15, 13}, {0, 1, 4}, {0, 1, 2}, {1, 12, 15}, {1, 15, 12}, {0, 1, 2}, {1, 11, 15}, {1, 15, 11},
    {0, 1, 6}, {0, 1, 2}, {1, 15, 10}, {0, 1, 2}, {1, 10, 15}, {1, 9, 15}, {0, 1, 2}, {1, 15, 9}, {1, 15, 8}, {0, 1, 10},
    {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 8, 15}, {1, 7, 15}, {1, 15, 7}, {0, 1, 2}, {1, 6, 15}, {1, 15, 6}, {0, 1, 4},
    {0, 1, 2}, {1, 5, 15}, {1, 15, 5}, {0, 1, 2}, {1, 4, 15}, {1, 15, 4}, {0, 1, 72}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 3, 15}, {1, 15, 3}, {0, 1, 2}, {1, 2, 15}, {1, 15, 2}, {0, 1, 6}, {0, 1, 2}, {1, 15, 1}, {0, 1, 2}, {1, 1, 15},
    {1, 15, 0}, {0, 1, 26}, {0, 1, 10}, {0, 1, 2}, {1, 0, 15}, {0, 1, 4}, {0, 1, 2}, {1, 14, 14}, {1, 13, 14}, {0, 1, 2},
    {1, 14, 13}, {1, 12, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 14, 12}, {1, 13, 13}, {0, 1, 2}, {1, 11, 14}, {1, 14, 11},
    {0, 1, 4}, {0, 1, 2}, {1, 12, 13}, {1, 13, 12}, {0, 1, 2}, {1, 10, 14}, {1, 14, 10}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 11, 13}, {1, 13, 11}, {0, 1, 2}, {1, 12, 12}, {1, 9, 14}, {0, 1, 4}, {0, 1, 2}, {1, 14, 9}, {1, 10, 13},
    {0, 1, 2}, {1, 13, 10}, {1, 11, 12}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 12, 11}, {1, 8, 14}, {0, 1, 2}, {1, 14, 8},
    {1, 9, 13}, {0, 1, 4}, {0, 1, 2}, {1, 13, 9}, {1, 7, 14}, {0, 1, 2}, {1, 14, 7}, {1, 10, 12}, {1, 15, 15}, {0, 1, 236},
    {0, 1, 144}, {0, 1, 78}, {0, 1, 46}, {0, 1, 26}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 12, 10}, {1, 11, 11},
    {0, 1, 2}, {1, 8, 13}, {1, 13, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 0, 14}, {1, 14, 0}, {1, 0, 13}, {1, 14, 6},
    {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 6, 14}, {1, 9, 12}, {1, 12, 9}, {0, 1, 2}, {1, 5, 14}, {1, 11, 10}, {0, 1, 10},
    {0, 1, 6}, {0, 1, 2}, {1, 14, 5}, {0, 1, 2}, {1, 10, 11}, {1, 7, 13}, {0, 1, 2}, {1, 13, 7}, {1, 14, 4}, {0, 1, 4},
    {0, 1, 2}, {1, 8, 12}, {1, 12, 8}, {0, 1, 4}, {0, 1, 2}, {1, 4, 14}, {1, 2, 14}, {1, 3, 14}, {0, 1, 16}, {0, 1, 8},
    {0, 1, 4}, {0, 1, 2}, {1, 6, 13}, {1, 13, 6}, {0, 1, 2}, {1, 14, 3}, {1, 9, 11}, {0, 1, 4}, {0, 1, 2}, {1, 11, 9},
    {1, 10, 10}, {0, 1, 2}, {1, 14, 2}, {1, 1, 14}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 14, 1}, {1, 5, 13}, {0, 1, 2},
    {1, 13, 5}, {1, 7, 12}, {0, 1, 4}, {0, 1, 2}, {1, 12, 7}, {1, 4, 13}, {0, 1, 2}, {1, 8, 11}, {1, 11, 8}, {0, 1, 32},
    {0, 1, 16}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 13, 4}, {1, 9, 10}, {0, 1, 2}, {1, 10, 9}, {1, 6, 12}, {0, 1, 4},
    {0, 1, 2}, {1, 12, 6}, {1, 3, 13}, {0, 1, 2}, {1, 13, 3}, {1, 2, 13}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 13, 2},
    {1, 1, 13}, {0, 1, 2}, {1, 7, 11}, {1, 11, 7}, {0, 1, 4}, {0, 1, 2}, {1, 13, 1}, {1, 5, 12}, {0, 1, 2}, {1, 12, 5},
    {1, 8, 10}, {0, 1, 18}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 10, 8}, {1, 9, 9}, {0, 1, 2}, {1, 4, 12}, {1, 12, 4},
    {0, 1, 4}, {0, 1, 2}, {1, 6, 11}, {1, 11, 6}, {0, 1, 4}, {0, 1, 2}, {1, 13, 0}, {1, 0, 12}, {1, 3, 12}, {0, 1, 8},
    {0, 1, 4}, {0, 1, 2}, {1, 12, 3}, {1, 7, 10}, {0, 1, 2}, {1, 10, 7}, {1, 2, 12}, {0, 1, 4}, {0, 1, 2}, {1, 12, 2},
    {1, 5, 11}, {0, 1, 2}, {1, 11, 5}, {1, 1, 12}, {0, 1, 58}, {0, 1, 36}, {0, 1, 20}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2},
    {1, 8, 9}, {1, 9, 8}, {0, 1, 2}, {1, 12, 1}, {1, 4, 11}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 12, 0}, {1, 0, 11},
    {1, 3, 11}, {0, 1, 4}, {0, 1, 2}, {1, 11, 0}, {1, 0, 10}, {1, 1, 10}, {0, 1, 6}, {0, 1, 2}, {1, 11, 4}, {0, 1, 2},
    {1, 6, 10}, {1, 10, 6}, {0, 1, 4}, {0, 1, 2}, {1, 7, 9}, {1, 9, 7}, {0, 1, 4}, {0, 1, 2}, {1, 10, 0}, {1, 0, 9},
    {1, 9, 0}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 11, 3}, {1, 8, 8}, {0, 1, 4}, {0, 1, 2}, {1, 2, 11}, {1, 5, 10},
    {1, 11, 2}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 10, 5}, {1, 1, 11}, {0, 1, 2}, {1, 11, 1}, {1, 6, 9}, {0, 1, 2},
    {1, 9, 6}, {1, 10, 4}, {0, 1, 18}, {0, 1, 10}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 4, 10}, {1, 7, 8}, {1, 8, 7},
    {0, 1, 2}, {1, 3, 10}, {1, 10, 3}, {0, 1, 4}, {0, 1, 2}, {1, 5, 9}, {1, 9, 5}, {0, 1, 2}, {1, 2, 10}, {1, 10, 2},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 10, 1}, {1, 6, 8}, {0, 1, 2}, {1, 8, 6}, {1, 7, 7}, {0, 1, 4}, {0, 1, 2},
    {1, 4, 9}, {1, 9, 4}, {0, 1, 2}, {1, 3, 9}, {1, 9, 3}, {0, 1, 64}, {0, 1, 32}, {0, 1, 16}, {0, 1, 8}, {0, 1, 4},
    {0, 1, 2}, {1, 5, 8}, {1, 8, 5}, {0, 1, 2}, {1, 2, 9}, {1, 6, 7}, {0, 1, 4}, {0, 1, 2}, {1, 7, 6}, {1, 9, 2},
    {0, 1, 2}, {1, 1, 9}, {1, 9, 1}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 4, 8}, {1, 8, 4}, {0, 1, 2}, {1, 5, 7},
    {1, 7, 5}, {0, 1, 4}, {0, 1, 2}, {1, 3, 8}, {1, 8, 3}, {0, 1, 2}, {1, 6, 6}, {1, 2, 8}, {0, 1, 18}, {0, 1, 8},
    {0, 1, 4}, {0, 1, 2}, {1, 8, 2}, {1, 1, 8}, {0, 1, 2}, {1, 4, 7}, {1, 7, 4}, {0, 1, 6}, {0, 1, 2}, {1, 8, 1},
    {0, 1, 2}, {1, 0, 8}, {1, 8, 0}, {0, 1, 2}, {1, 5, 6}, {1, 6, 5}, {0, 1, 8}, {0, 1, 6}, {0, 1, 2}, {1, 1, 7},
    {0, 1, 2}, {1, 0, 7}, {1, 7, 0}, {1, 7, 3}, {0, 1, 4}, {0, 1, 2}, {1, 3, 7}, {1, 2, 7}, {1, 7, 2}, {0, 1, 16},
    {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 4, 6}, {1, 6, 4}, {0, 1, 2}, {1, 5, 5}, {1, 7, 1}, {0, 1, 4}, {0, 1, 2},
    {1, 3, 6}, {1, 6, 3}, {0, 1, 2}, {1, 4, 5}, {1, 5, 4}, {0, 1, 8}, {0, 1, 4}, {0, 1, 2}, {1, 2, 6}, {1, 6, 2},
    {0, 1, 2}, {1, 1, 6}, {1, 6, 1}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 0, 6}, {1, 6, 0}, {1, 3, 5}, {0, 1, 2},
    {1, 5, 3}, {1, 4, 4}, {0, 1, 52}, {0, 1, 38}, {0, 1, 24}, {0, 1, 16}, {0, 1, 10}, {0, 1, 4}, {0, 1, 2}, {1, 2, 5},
    {1, 5, 2}, {0, 1, 2}, {1, 1, 5}, {0, 1, 2}, {1, 0, 5}, {1, 5, 0}, {0, 1, 2}, {1, 5, 1}, {0, 1, 2}, {1, 3, 4},
    {1, 4, 3}, {0, 1, 4}, {0, 1, 2}, {1, 2, 4}, {1, 4, 2}, {0, 1, 2}, {1, 3, 3}, {1, 1, 4}, {0, 1, 10}, {0, 1, 6},
    {0, 1, 2}, {1, 4, 1}, {0, 1, 2}, {1, 0, 4}, {1, 4, 0}, {0, 1, 2}, {1, 2, 3}, {1, 3, 2}, {0, 1, 2}, {1, 1, 3},
    {1, 3, 1}, {0, 1, 8}, {0, 1, 6}, {0, 1, 4}, {0, 1, 2}, {1, 0, 3}, {1, 3, 0}, {1, 2, 2}, {1, 1, 2}, {0, 1, 2},
    {1, 2, 1}, {0, 1, 2}, {1, 0, 2}, {1, 2, 0}, {0, 1, 4}, {0, 1, 2}, {1, 1, 1}, {1, 0, 1}, {0, 1, 2}, {1, 1, 0},
    {1, 0, 0}
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
    {NULL, 0}, // table 0 is empty
    {s_table1, 0},
    {s_table2, 0},
    {s_table3, 0},
    {NULL, 0}, // table 4 is not used
    {s_table5, 0},
    {s_table6, 0},
    {s_table7, 0},
    {s_table8, 0},
    {s_table9, 0},
    {s_table10, 0},
    {s_table11, 0},
    {s_table12, 0},
    {s_table13, 0},
    {NULL, 0}, // table 14 is not used
    {s_table15, 0},
    {s_table16, 1},
    {s_table16, 2}, // 17
    {s_table16, 3}, // 18
    {s_table16, 4}, // 19
    {s_table16, 6}, // 20
    {s_table16, 8}, // 21
    {s_table16, 10}, // 22
    {s_table16, 13}, // 23
    {s_table24, 4},
    {s_table24, 5}, // 25
    {s_table24, 6}, // 26
    {s_table24, 7}, // 27
    {s_table24, 8}, // 28
    {s_table24, 9}, // 29
    {s_table24, 11}, // 30
    {s_table24, 13}, // 31
};

const HuffmanDecoder::HuffmanTableCount1 HuffmanDecoder::s_count1HuffmanTables[HuffmanDecoder::s_count1HuffmanTableCount] =
{
    {s_count1table0},
    {s_count1table1}
};

// =====================================================================================================================
static inline int DecodeLinbits(BitStream& bs, const HuffmanDecoder::HuffmanTable& table, int value)
{
    // this table is not using linbits
    if (table.m_linbits == 0)
	return value;

    // append linbits to the value
    if (value == 15)
	value += bs.GetData(table.m_linbits);

    return value;
}

// =====================================================================================================================
static inline int DecodeSign(BitStream& bs, int value)
{
    // no sign bit for zero value
    if (value == 0)
	return value;

    // '1' bit means negative value
    if (bs.GetData(1) == 1)
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

    // X value
    int val = DecodeLinbits(bs, table, data->m_value1);
    val = DecodeSign(bs, val);
    samples.push_back(val);

    // Y value
    val = DecodeLinbits(bs, table, data->m_value2);
    val = DecodeSign(bs, val);
    samples.push_back(val);
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
