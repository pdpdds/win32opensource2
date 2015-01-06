// popcapEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "PakInterface.h"

int _tmain(int argc, _TCHAR* argv[])
{
	gPakInterface->AddPakFile("packing.pak");

	PFILE* pTest1File = p_fopen("pack1.txt", "rb");
	if (pTest1File == NULL)
		exit(0);

	p_fseek(pTest1File, 0, SEEK_END);
	int size = p_ftell(pTest1File);
	p_fseek(pTest1File, 0, SEEK_SET);

	char* pBuffer = new char[size + 1];

	p_fread(pBuffer, sizeof(char), size, pTest1File);
	p_fclose(pTest1File);

	pBuffer[size] = 0;
	printf("%s\n", pBuffer);

	return 0;
}

