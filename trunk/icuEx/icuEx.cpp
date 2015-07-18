// icuEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "unicode/ustring.h"
#include "unicode/utypes.h"
#include "unicode/uloc.h"
#include "unicode/ucol.h"

#pragma comment(lib, "icuucd.lib")

int _tmain(int argc, _TCHAR* argv[])
{

	std::ifstream f("config.xml");
	std::string s;
	while (std::getline(f, s)) {
		// at this point s contains a line of text
		// which may be ANSI or UTF-8 encoded

		// convert std::string to ICU's UnicodeString
		UnicodeString ucs = UnicodeString::fromUTF8(StringPiece(s.c_str()));

		// convert UnicodeString to std::wstring
		std::wstring ws;
		for (int i = 0; i < ucs.length(); ++i)
			ws += static_cast<wchar_t>(ucs[i]);
	}

	return 0;
}

