#ifndef BITSTREAM_H_INCLUDED
#define BITSTREAM_H_INCLUDED

#include <vector>

class BitStream
{
    public:
	BitStream(const std::vector<unsigned char>& data, unsigned size);

	unsigned GetPosition() const;

	unsigned GetData(unsigned bitLength);

	void Rewind(unsigned amount);

    private:
	/// The actual data to read from
	const std::vector<unsigned char>& m_data;
	/// Number of available bits in the data
	unsigned m_size;

	/// Current byte position
	unsigned m_bytePos;
	/// Current bit position inside the byte
	unsigned m_bitPos;
};

#endif
