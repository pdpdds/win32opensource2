// cppformatEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "format.h"

#pragma comment(lib, "format.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	int sec = 3722; //1�ð� 2�� 2��
	int hour = sec / (60 * 60);
	int minute = (sec / 60 * 60) / 60;
	int second = sec % 60;

	std::string szTime = fmt::format("{0:02}:{1:02}:{2:02}", hour, minute, second);
	fmt::print("Time is {0}.", szTime);

	getchar();
	return 0;
}

