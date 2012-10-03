#ifndef CODEC_MP3_FRAMEHEADER_H_INCLUDED
#define CODEC_MP3_FRAMEHEADER_H_INCLUDED

#include <stdexcept>
#include <stdint.h>

namespace codec
{
    namespace mp3
    {
	/**
	 * Exception thrown in case of invalid data was found during parsing the frame header.
	 */
	class InvalidFrameHeaderException : public std::runtime_error
	{
	    public:
		InvalidFrameHeaderException(const std::string& error)
		    : std::runtime_error(error)
		{}
	};

	/**
	 * This class is used to parse and store the contents of an MP3 frame header.
	 */
	class FrameHeader
	{
	    public:
		struct NotInitialized {};

		FrameHeader(const NotInitialized& dummy);

		/**
		 * Creates a frame header object from the given raw data.
		 *
		 * InvalidFrameHeaderException is thrown if invalid data was given.
		 */
		static FrameHeader Parse(const uint8_t* data);

		enum ChannelMode
		{
		    STEREO,
		    JOINT_STEREO,
		    DUAL_CHANNEL,
		    SINGLE_CHANNEL
		};

		/// returns layer number
		unsigned GetLayer() const
		{ return m_layer; }

		/// returns bitrate
		unsigned GetBitrate() const
		{ return m_bitrate; }

		/// returns sampling frequency
		unsigned GetSamplingRate() const
		{ return m_sampling; }

		/// returns channel mode
		ChannelMode GetChannelMode() const
		{ return m_channelMode; }

		/// returns true if the current frame contains mono audio
		bool IsMono() const
		{ return m_channelMode == SINGLE_CHANNEL; }

		/// returns whether the frame is protected with CRC
		bool IsProtected() const
		{ return m_crc; }

		/// calculates the size of the current frame according to the parsed parameters
		unsigned CalculateFrameSize() const;

	    private:
		FrameHeader(unsigned layer, unsigned bitrate, unsigned sampling, ChannelMode chanMode, bool crc, bool padded);

		// MPEG layer number
		unsigned m_layer;
		// bitrate
		unsigned m_bitrate;
		// sampling frequency
		unsigned m_sampling;
		// channel mode
		ChannelMode m_channelMode;
		// true if the frame is protected with CRC
		bool m_crc;
		// true if the frame is padded with one byte
		bool m_padded;

		// bitrate table for MPEG version 1
		static const unsigned s_bitrates_v1[3][16];
		// sampling frequency table for MPEG version 1
		static const unsigned s_sampling_v1[4];
	};

	std::ostream& operator<<(std::ostream& s, const FrameHeader& header);
    }
}

#endif
