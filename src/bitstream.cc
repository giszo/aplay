#include <bitstream.h>

#include <iostream>
#include <stdlib.h>

BitStream::BitStream(const std::vector<unsigned char>& data, unsigned size)
    : m_data(data),
      m_size(size),
      m_bytePos(0),
      m_bitPos(0)
{
}

unsigned BitStream::GetPosition() const
{
    return m_bytePos * 8 + m_bitPos;
}

unsigned BitStream::GetData(unsigned bitLength)
{
    // check that we have enough data to serve the request
    if (m_bytePos * 8 + m_bitPos + bitLength > m_size)
    {
	std::cerr << "Asked too much data from the bit stream!" << std::endl;
	std::cout << "Total data: " << m_size << " bits" << std::endl;
	std::cout << "Current position: " << (m_bytePos * 8 + m_bitPos) << std::endl;
	std::cout << "Requested amount: " << bitLength << std::endl;
	abort();
    }

    unsigned data = 0;

    // handle partial bits at the beginning
    if (m_bitPos != 0)
    {
	unsigned amount = std::min(bitLength, 8 - m_bitPos);
	unsigned remaining = 8 - (m_bitPos + amount);

	data <<= amount;
	data |= (m_data[m_bytePos] >> remaining) & ((1 << amount) - 1);

	m_bitPos = (m_bitPos + amount) % 8;
	if (m_bitPos == 0)
	    m_bytePos++;

	bitLength -= amount;
    }

    // handle complete aligned bytes
    while (bitLength >= 8)
    {
	data <<= 8;
	data |= m_data[m_bytePos++];
	bitLength -= 8;
    }

    // handle partial bits at the end
    if (bitLength > 0)
    {
	unsigned remaining = 8 - bitLength;

	data <<= bitLength;
	data |= (m_data[m_bytePos] >> remaining) & ((1 << bitLength) - 1);

	m_bitPos = bitLength;
    }

    return data;
}

void BitStream::Rewind(unsigned amount)
{
    if (m_bitPos > 0)
    {
	unsigned i = std::min(amount, m_bitPos);

	m_bitPos -= i;
	amount -= i;
    }

    while (amount >= 8)
    {
	m_bytePos--;
	amount -= 8;
    }

    if (amount > 0)
	m_bitPos = 8 - amount;
}
