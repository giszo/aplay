#ifndef BITSTREAM_H_INCLUDED
#define BITSTREAM_H_INCLUDED

#include <vector>

class BitStream
{
    public:
	typedef std::vector<unsigned char>::const_iterator StreamIterator;

	BitStream(const StreamIterator& start, const StreamIterator& end);

	/// returns the length of the bitstream
	unsigned GetLength() const;
	/// returns the current position in the bitstream
	unsigned GetPosition() const;

	unsigned GetData(unsigned bitLength);

	void Skip(unsigned amount);
	void Rewind(unsigned amount);

    private:
	/// start of the bitstream
	StreamIterator m_start;
	/// end of the bitstream
	StreamIterator m_end;

	/// current position in the bitstream
	StreamIterator m_pos;

	/// bit position inside the current byte of the stream
	unsigned m_bitPos;
};

#endif
