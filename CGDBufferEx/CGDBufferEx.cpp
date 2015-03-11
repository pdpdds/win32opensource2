// CGDBufferEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <CGDbuffer>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	CGD::buffer	buffer(new char[256]);
	char* szSampleStr = "CGD Buffer Test!!";

	buffer.append<int>(1234);
	buffer.append<short>(-184);
	buffer.append<char*>(szSampleStr);
	buffer.append<int>(sizeof(szSampleStr));
	buffer.append<char>('a');
	
	CGD::POS pos;

	auto	intValue = buffer.front<int>(pos);	
	auto	shortValue = buffer.front<short>(pos);
	auto	szStringValue = buffer.front<char*>(pos);
	pos += 4;
	auto	charValue = buffer.front<char>(pos);

	// 확인) 값을 제대로 읽었나 찍어본다.
	cout << intValue << endl;
	cout << shortValue << endl;
	cout << szStringValue << endl;
	cout << charValue << endl;

	delete buffer.clear();

	return 0;
}

