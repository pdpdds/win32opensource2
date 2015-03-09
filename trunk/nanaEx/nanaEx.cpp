// nanaEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <nana/gui.hpp>

using namespace nana;

#ifdef _DEBUG
#pragma comment(lib, "nana_debug.lib")
#else
#pragma comment(lib, "nana_release.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	form fm;
	drawing{ fm }.draw([](paint::graphics& graph){
		graph.string({ 10, 10 }, L"Hello, world!", colors::red);
	});
	fm.events().click(API::exit);
	fm.show();
	exec();

	return 0;
}

