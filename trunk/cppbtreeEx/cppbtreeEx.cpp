// cppbtreeEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

