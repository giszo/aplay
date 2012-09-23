#ifndef FILEDATASOURCE_H_INCLUDED
#define FILEDATASOURCE_H_INCLUDED

#include "datasource.h"

#include <string>

class FileDataSource : public DataSource
{
    public:
	FileDataSource(const std::string& filename);
	virtual ~FileDataSource();

	virtual int Read(void* data, int size);

	virtual off_t GetPosition() const;

    private:
	int m_fd;
};

#endif
