#include <bitstream.h>

#include <iostream>
#include <cassert>
#include <cstdlib>

// =====================================================================================================================
BitStream::BitStream(const StreamIterator& start, const StreamIterator& end)
    : m_start(start),
      m_end(end),
      m_pos(start),
      m_bitPos(0)
{
}

// =====================================================================================================================
unsigned BitStream::GetLength() const
{
    return (m_end - m_start) * 8;
}

// =====================================================================================================================
unsigned BitStream::GetPosition() const
{
    return (m_pos - m_start) * 8 + m_bitPos;
}

// =====================================================================================================================
unsigned BitStream::GetData(unsigned bitLength)
{
    // make sure we can return the requested data in the 'unsigned' type
    assert(bitLength <= 32);

    if (GetPosition() + bitLength > GetLength())
    {
	std::cerr << "Asked too much data from the bit stream!" << std::endl;
	std::cerr << "Total length: " << GetLength() << std::endl;
	std::cerr << "Current position: " << GetPosition() << std::endl;
	std::cerr << "Number of requested bits: " << bitLength << std::endl;
	abort();
    }

    unsigned data = 0;

    // handle partial bits at the beginning
    if (m_bitPos != 0)
    {
	unsigned amount = std::min(bitLength, 8 - m_bitPos);
	unsigned remaining = 8 - (m_bitPos + amount);

	register unsigned char streamData = *m_pos;

	data <<= amount;
	data |= (streamData >> remaining) & ((1 << amount) - 1);

	m_bitPos = (m_bitPos + amount) % 8;

	if (m_bitPos == 0)
	    ++m_pos;

	bitLength -= amount;
    }

    // handle complete aligned bytes
    while (bitLength >= 8)
    {
	data <<= 8;
	data |= *m_pos++;
	bitLength -= 8;
    }

    // handle partial bits at the end
    if (bitLength > 0)
    {
	unsigned remaining = 8 - bitLength;

	register unsigned char streamData = *m_pos;

	data <<= bitLength;
	data |= (streamData >> remaining) & ((1 << bitLength) - 1);

	m_bitPos = bitLength;
    }

    return data;
}

// =====================================================================================================================
void BitStream::Skip(unsigned amount)
{
    if (GetPosition() + amount > GetLength())
    {
	abort();
    }

    unsigned bits = amount + m_bitPos;

    m_pos += bits / 8;
    m_bitPos = bits % 8;
}

// =====================================================================================================================
void BitStream::Rewind(unsigned amount)
{
    if (amount > GetPosition())
    {
	std::cerr << "Tried to rewind bit stream with too big number of bits!" << std::endl;
	std::cerr << "Current position: " << GetPosition() << std::endl;
	std::cerr << "Number of bits to rewind: " << amount << std::endl;
	abort();
    }

    if (m_bitPos > 0)
    {
	unsigned i = std::min(amount, m_bitPos);

	m_bitPos -= i;
	amount -= i;
    }

    while (amount >= 8)
    {
	--m_pos;
	amount -= 8;
    }

    if (amount > 0)
    {
	--m_pos;
	m_bitPos = 8 - amount;
    }
}
