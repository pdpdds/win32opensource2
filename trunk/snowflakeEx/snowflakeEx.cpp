// snowflakeEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

