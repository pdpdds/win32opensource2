

#include "unicode/utypes.h"

#include "unicode/caniter.h"
#include "unicode/normalizer2.h"
#include "unicode/uchar.h"
#include "unicode/uniset.h"
#include "unicode/usetiter.h"
#include "unicode/ustring.h"
#include "unicode/utf16.h"


#include <iostream>
#include <fstream>
#include <string>

U_NAMESPACE_BEGIN
#pragma comment(lib, "icuucd.lib")


int main(int argc, char* argv[])
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

U_NAMESPACE_END

int main(int argc, char* argv[])
{
	icu_69::main(argc, argv);
	return 0;
}
