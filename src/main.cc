#include <filedatasource.h>
#include <codec/mp3/mp3.h>
#include <bitstream.h>

#include <iostream>

int main(int argc, char** argv)
{
    DataSource* src = new FileDataSource("test.mp3");
    Mp3Codec* codec = new Mp3Codec(src);

    codec->ReadFrame();
//    codec->ReadFrame();

    return 0;
}
