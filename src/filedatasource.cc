#include <filedatasource.h>

#include <fcntl.h>
#include <unistd.h>

FileDataSource::FileDataSource(const std::string& filename)
{
    m_fd = ::open(filename.c_str(), O_RDONLY);
}

FileDataSource::~FileDataSource()
{
    ::close(m_fd);
}

int FileDataSource::Read(void* data, int size)
{
    return ::read(m_fd, data, size);
}

off_t FileDataSource::GetPosition() const
{
    return lseek(m_fd, 0, SEEK_CUR);
}
