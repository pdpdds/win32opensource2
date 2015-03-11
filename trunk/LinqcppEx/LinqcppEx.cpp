// LinqcppEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "linq.h"
#include <stdio.h>
#include <string>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	struct {
		string Name;
		string Date;
	} dateArray[] = {
		{
			"Smy20011", "2012"
		},
		{
			"Wang Huan", "2013"
		}
	};
	auto result = From(dateArray).Where([](decltype(dateArray[0]) item) {return item.Date == "2013"; });

	for (auto i : result)
	{
		printf(" %s ", i.Name.c_str());
	}

	return 0;
}