// cppformatEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "format.h"

#pragma comment(lib, "format.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	int sec = 3722; //1시간 2분 2초
	int hour = sec / (60 * 60);
	int minute = (sec / 60 * 60) / 60;
	int second = sec % 60;

	std::string szTime = fmt::format("{0:02}:{1:02}:{2:02}", hour, minute, second);
	fmt::print("Time is {0}.", szTime);

	getchar();
	return 0;
}

