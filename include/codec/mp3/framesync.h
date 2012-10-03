#ifndef CODEC_MP3_FRAMESYNC_H_INCLUDED
#define CODEC_MP3_FRAMESYNC_H_INCLUDED

#include "frameheader.h"

#include <datasource.h>

#include <stdint.h>

namespace codec
{
    namespace mp3
    {
	/**
	 * Synchronization helper to find the beginning of the next MP3 frame in the data source.
	 */
	class FrameSynchronizer
	{
	    public:
		FrameSynchronizer(DataSource& dataSource);

		/**
		 * Synchronizes the data source to the next MP3 frame.
		 *
		 * A successful synchronization will position the data source at the end of the next MP3 frame header.
		 * The contents of the frame header will be read and can be reached with the appropriate getter.
		 *
		 * @return true is returned if frame synchron was found
		 */
		bool Sync();

		/// returns the header of the last frame
		const FrameHeader& GetFrameHeader() const
		{ return m_frameHeader; }

	    private:
		/// data source that will be synchronized to the beginning of the next MP3 frame
		DataSource& m_dataSource;

		/// size of an MP3 frame header
		static const int s_frameHeaderSize = 4;

		/// contents of the found MP3 frame header
		uint8_t m_headerData[s_frameHeaderSize];

		/// header of the latest frame the source was synchronized to
		FrameHeader m_frameHeader;
	};
    }
}

#endif
