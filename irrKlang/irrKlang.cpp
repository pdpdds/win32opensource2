#include <iostream>
#include <irrKlang.h>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

int main(int argc, const char** argv)
{
	//���� ������ ����
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; 

	//mp3 ������ �����Ѵ�.
	engine->play2D("sample.mp3", true);

	char i = 0;
	std::cin >> i; //���Ḧ ���� Ű �Է��� ����Ѵ�.

	engine->drop(); //���� ��ü�� �����Ѵ�.
	return 0;
}