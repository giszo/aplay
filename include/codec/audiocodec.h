#ifndef AUDIOCODEC_H_INCLUDED
#define AUDIOCODEC_H_INCLUDED

#include "../datasource.h"

class AudioCodec
{
    public:
	AudioCodec(DataSource* source)
	    : m_source(source)
	{}

	virtual ~AudioCodec()
	{}

    protected:
	DataSource* m_source;
};

#endif
