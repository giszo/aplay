#ifndef TEST_FAKEDATASOURCE_H_INCLUDED
#define TEST_FAKEDATASOURCE_H_INCLUDED

#include <datasource.h>

namespace test
{

class FakeDataSource : public DataSource
{
    public:
	FakeDataSource(const std::vector<unsigned char>& data)
	    : m_data(data)
	{
	    m_position = m_data.begin();
	}

	virtual int Read(void* data, int size)
	{
	    int amount = std::min(size, m_data.end() - m_position);

	    if (amount > 0)
	    {
		memcpy(data, &*m_position, amount);
		m_position += amount;
	    }

	    return amount;
	}

	virtual off_t GetPosition() const
	{
	    return m_position - m_data.begin();
	}

    private:
	const std::vector<unsigned char>& m_data;

	std::vector<unsigned char>::const_iterator m_position;
};

}

#endif
