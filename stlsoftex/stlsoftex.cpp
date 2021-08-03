#include <stlsoft/memory/auto_buffer.hpp>
#include <iostream>

//자동 버퍼 사용예제
int main(int argc, char** argv) 
{
    try
    {
        stlsoft::auto_buffer<char, 64>  buff(0);

        // 버퍼 크기를 20바이트로 변경한다.
        buff.resize(20);

        ::memset(&buff[0], 1, buff.size());

        // 버퍼크기를 300바이트로 변경한다.
        buff.resize(300);

        ::memset(&buff[0], 2, buff.size());
    }
    catch (std::exception& x)
    {
        std::cerr << "Error: " << x.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error" << std::endl;
    }

    return 0;
}