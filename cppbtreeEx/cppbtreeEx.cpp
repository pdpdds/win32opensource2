// cppbtreeEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "btree_Set.h"

using namespace btree;

int _tmain(int argc, _TCHAR* argv[])
{
	btree_set<int32_t> elements;
	
	elements.insert(5);
	elements.insert(9);
	elements.insert(7);
	elements.insert(8);
	elements.insert(2);

	for (auto element : elements)
	{
		printf("%d ", element);
	}

	return 0;
}

