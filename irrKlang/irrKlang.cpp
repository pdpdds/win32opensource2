#include <iostream>
#include <irrKlang.h>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

int main(int argc, const char** argv)
{
	//사운드 엔진의 생성
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; 

	//mp3 파일을 연주한다.
	engine->play2D("sample.mp3", true);

	char i = 0;
	std::cin >> i; //종료를 위해 키 입력을 대기한다.

	engine->drop(); //엔진 객체를 제거한다.
	return 0;
}