// mpirEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <mpir.h>

#pragma comment(lib, "mpir.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	mpz_t a, b, c;
	mpz_init(a); mpz_init(b); mpz_init(c);

	mpz_set_str(a, "1024", 10);
	mpz_set_str(b, "768", 10);
	mpz_gcd(c, a, b);

	printf("GCD of "); mpz_out_str(stdout, 10, a);
	printf(" and ");   mpz_out_str(stdout, 10, b);
	printf(" is ");    mpz_out_str(stdout, 10, c);
	putchar('\n');

	return 0;
}

