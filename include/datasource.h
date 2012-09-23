#ifndef DATASOURCE_H_INCLUDED
#define DATASOURCE_H_INCLUDED

#include <sys/types.h>

class DataSource
{
    public:
	virtual ~DataSource()
	{}

	virtual int Read(void* data, int size) = 0;

	virtual off_t GetPosition() const = 0;
};

#endif
