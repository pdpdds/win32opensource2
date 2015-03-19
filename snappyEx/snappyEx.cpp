// snappyEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "snappy-c.h"
#include <assert.h>

#pragma comment(lib, "snappy32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	FILE* f = NULL;
	errno_t err = fopen_s(&f, "readme.txt", "rb");
	fseek(f, 0, SEEK_END);
	int fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* source = new char[fileSize];

	int bufferLen = fileSize + 300;
	char* compressed = new char[bufferLen];
	fread(source, fileSize, 1, f);

	fclose(f);

	size_t length = bufferLen;
	snappy_status status = snappy_compress(source, fileSize, compressed, &length);

	delete source;

	char* uncompressed = new char[bufferLen];
	size_t uncompressedLength = bufferLen;
	status = snappy_uncompress(compressed, length, uncompressed, &uncompressedLength);

	assert(uncompressedLength == fileSize);

	delete compressed;
	delete uncompressed;

	return 0;
}

