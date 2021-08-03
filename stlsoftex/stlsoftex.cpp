#include <stlsoft/memory/auto_buffer.hpp>
#include <iostream>

//�ڵ� ���� ��뿹��
int main(int argc, char** argv) 
{
    try
    {
        stlsoft::auto_buffer<char, 64>  buff(0);

        // ���� ũ�⸦ 20����Ʈ�� �����Ѵ�.
        buff.resize(20);

        ::memset(&buff[0], 1, buff.size());

        // ����ũ�⸦ 300����Ʈ�� �����Ѵ�.
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