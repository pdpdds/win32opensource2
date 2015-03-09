// snowflakeEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Snowflake.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Snowflake* snow = new Snowflake();
	snow->setEpoch(1367505795100);
	snow->setMachine(30);

	while (true) {
		printf(" %lld\n", snow->generate());
	}

	return 0;
}

