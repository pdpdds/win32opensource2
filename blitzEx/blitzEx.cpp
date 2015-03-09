// blitzEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
//2차원 정수 배열을 선언한다.
//3x3 매트릭스다.
	Array<int, 2> B(3, 3);

//값의 초기화
	B = 0, 1, 2,
		3, 4, 5,
		6, 7, 8;

	cout << B << endl;

//매트릭스(2차원 배열)을 transpose한다.
	B.transposeSelf(secondDim, firstDim);

	cout << B << endl;

	return 0;
}

