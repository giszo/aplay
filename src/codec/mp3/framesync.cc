#include <codec/mp3/framesync.h>
#include <codec/mp3/frameheader.h>

using codec::mp3::FrameSynchronizer;
using codec::mp3::FrameHeader;
using codec::mp3::InvalidFrameHeaderException;

// =====================================================================================================================
FrameSynchronizer::FrameSynchronizer(DataSource& dataSource)
    : m_dataSource(dataSource),
      m_frameHeader(FrameHeader::NotInitialized())
{
}

// =====================================================================================================================
bool FrameSynchronizer::Sync()
{
    // read the first chunk of the data that could contain a valid frame header
    if (m_dataSource.Read(m_headerData, s_frameHeaderSize) != s_frameHeaderSize)
	return false;

    while (1)
    {
	try
	{
	    // try to parse the frame header
	    m_frameHeader = FrameHeader::Parse(m_headerData);
	    return true;
	}
	catch (const InvalidFrameHeaderException& e)
	{
	    // frame header is invalid ...

	    // drop the first byte of the current header
	    for (unsigned i = 0; i < s_frameHeaderSize - 1; ++i)
		m_headerData[i] = m_headerData[i + 1];

	    // read a new byte to the end of the buffer and try to parse again
	    if (m_dataSource.Read(&m_headerData[s_frameHeaderSize - 1], 1) != 1)
		return false;
	}
    }
}
