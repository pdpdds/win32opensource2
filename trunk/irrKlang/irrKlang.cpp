// irrKlang.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <irrKlang.h>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

int main(int argc, const char** argv)
{
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; // error starting up the engine

	// play some sound stream, looped
	engine->play2D("sample.mp3", true);

	char i = 0;
	std::cin >> i; // wait for user to press some key

	engine->drop(); // delete engine
	return 0;
}