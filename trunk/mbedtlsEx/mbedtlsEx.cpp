// mbedtlsEx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "polarssl/config.h"
#include "polarssl/platform.h"
#include "polarssl/md5.h"

#pragma comment(lib, "mbedTLS.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	unsigned char digest[16];
	char str[] = "Hello, world!";

	polarssl_printf("\n  MD5('%s') = ", str);

	md5((unsigned char *)str, 13, digest);

	for (i = 0; i < 16; i++)
		polarssl_printf("%02x", digest[i]);

	polarssl_printf("\n\n");

	return 0;
}

