// mcbdesEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "McbDES2.hpp"
#include <string>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char * lpKey1 = (unsigned char*)"11111111";
	unsigned char * lpKey2 = (unsigned char*)"22222222";

	/*
	*************************************************************************
	* Perform triple DES encryption = allow the class to manage the memory.
	*************************************************************************
	*/
	McbDES desEncrypt;

	/*
	*************************************************************************
	* Both keys are required for triple DES.
	*************************************************************************
	*/
	desEncrypt.McbSetKey1(lpKey1);
	desEncrypt.McbSetKey2(lpKey2);

	if (desEncrypt.McbEncrypt("Encrypted with triple DES"))
	{
		/*
		*********************************************************************
		* Perform triple DES decryption - use the buffer stored in the
		* instance which encrypted for convenience.
		*********************************************************************
		*/
		McbDES desDecrypt;

		desDecrypt.McbSetKey1(lpKey1);
		desDecrypt.McbSetKey2(lpKey2);

		desDecrypt.McbDecrypt(desEncrypt.McbGetCryptogram(),
			desEncrypt.McbGetCryptogramSize());

		printf("Decryption (%d) bytes: %s\n",
			desDecrypt.McbGetPlainTextSize(), desDecrypt.McbGetPlainText());
	}

	return 0;
}

