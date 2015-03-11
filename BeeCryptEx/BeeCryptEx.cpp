// BeeCryptEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>

#include "beecrypt/sha1.h"
#include "beecrypt/memchunk.h"

#ifdef _DEBUG
#pragma comment(lib, "beecryptd.lib")
#else
#pragma comment(lib, "beecrypt.lib")
#endif

struct vector
{
	int input_size;
	byte* input;
	byte* expect;
};

struct vector table[2] = {
	{ 3, (byte*) "abc",
	(byte*) "\xA9\x99\x3E\x36\x47\x06\x81\x6A\xBA\x3E\x25\x71\x78\x50\xC2\x6C\x9C\xD0\xD8\x9D" },
	{ 56, (byte*) "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	(byte*) "\x84\x98\x3E\x44\x1C\x3B\xD2\x6E\xBA\xAE\x4A\xA1\xF9\x51\x29\xE5\xE5\x46\x70\xF1" }
};

int main()
{
	int i, failures = 0;
	byte digest[20];
	sha1Param param;

	for (i = 0; i < 2; i++)
	{
		if (sha1Reset(&param))
			return -1;
		if (sha1Update(&param, table[i].input, table[i].input_size))
			return -1;
		if (sha1Digest(&param, digest))
			return -1;

		if (memcmp(digest, table[i].expect, 20))
		{
			printf("failed test vector %d\n", i + 1);
			failures++;
		}
	}
	return failures;
}
