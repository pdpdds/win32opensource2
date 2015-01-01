// ctemplateEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>  
#include <string>
#include <ctemplate/template.h>

#ifdef _DEBUG
#pragma comment(lib, "libctemplate-debug.lib")
#else
#pragma comment(lib, "libctemplate.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{

	ctemplate::TemplateDictionary dict("example");
	int winnings = rand() % 100000;
	dict["NAME"] = "John Smith";
	dict["VALUE"] = winnings;
	dict.SetFormattedValue("TAXED_VALUE", "%.2f", winnings * 0.83);
	
	if (1) {
		dict.ShowSection("IN_CA");
	}
	std::string output;
	ctemplate::ExpandTemplate("example.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
	std::cout << output;

	return 0;
}

