// opensslEx2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")



#define IN_FILE "plain.txt"

#define OUT_FILE "encrypt.bin"



unsigned char * readFile(char *fname, int *len);


int main(void){

	unsigned char key[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	unsigned char iv[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	BIO *errBIO = NULL;
	BIO *outBIO = NULL;

	ERR_load_crypto_strings();

	if ((errBIO = BIO_new(BIO_s_file())) != NULL)
		BIO_set_fp(errBIO, stderr, BIO_NOCLOSE | BIO_FP_TEXT);

	outBIO = BIO_new_file(OUT_FILE, "wb");

	if (!outBIO){
		BIO_printf(errBIO, "파일 [%s]을 생성하는데 에러가 발생했습니다.", OUT_FILE);
		ERR_print_errors(errBIO);

		exit(1);
	}

	int len;

	unsigned char * readBuffer = readFile(IN_FILE, &len);

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_bf_cbc(), NULL, key, iv);

	unsigned char * outbuf = (unsigned char *)malloc(sizeof(unsigned char)*(len + EVP_CIPHER_CTX_block_size(&ctx)));



	int outlen, tmplen;



	if (!EVP_EncryptUpdate(&ctx, outbuf, &outlen, readBuffer, strlen((char *)readBuffer))){

		return 0;

	}



	if (!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen))

		return  0;



	outlen += tmplen;



	EVP_CIPHER_CTX_cleanup(&ctx);

	BIO_write(outBIO, outbuf, outlen);


	BIO_free(outBIO);

	return 0;

}


unsigned char * readFile(char *fname, int *len){

	FILE *file;

	file = fopen(fname, "r");

	if (file == NULL){

		printf("Error of Reading\n");

	}

	long temp_len;

	unsigned char * buf;


	temp_len = _filelength(_fileno(file));

	printf("%d\n", temp_len);

	*len = (int)temp_len;

	buf = (unsigned char *)malloc(sizeof(*len));

	fread(buf, 1, *len, file);

	printf("%d\n", *len);

	return buf;

}

