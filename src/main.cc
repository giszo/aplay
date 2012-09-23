#include <filedatasource.h>
#include <codec/mp3.h>
#include <bitstream.h>

#include <iostream>

int main(int argc, char** argv)
{
#if 0
    std::vector<unsigned char> data;
    data.push_back(0x83);
    BitStream bs(data, 8);
    std::cout << bs.GetData(1) << " ";
    std::cout << bs.GetData(5) << " ";
    std::cout << bs.GetData(2) << std::endl;
#endif
//    for (unsigned i = 0; i < 8; ++i)
//	std::cout << "Bit #" << i << " is " << bs.GetData(1) << std::endl;

    DataSource* src = new FileDataSource("test.mp3");
    Mp3Codec* codec = new Mp3Codec(src);
    codec->ReadFrame();
    codec->ReadFrame();
    return 0;
}
