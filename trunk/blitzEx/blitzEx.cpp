// blitzEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <blitz/array.h>
#include <random/discrete-uniform.h>

BZ_USING_NAMESPACE(blitz)

#ifdef _DEBUG
#pragma comment(lib, "blitz_d.lib")
#else
#pragma comment(lib, "blitz.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
//2���� ���� �迭�� �����Ѵ�.
//3x3 ��Ʈ������.
	Array<int, 2> B(3, 3);

//���� �ʱ�ȭ
	B = 0, 1, 2,
		3, 4, 5,
		6, 7, 8;

	cout << B << endl;

//��Ʈ����(2���� �迭)�� transpose�Ѵ�.
	B.transposeSelf(secondDim, firstDim);

	cout << B << endl;

	return 0;
}

