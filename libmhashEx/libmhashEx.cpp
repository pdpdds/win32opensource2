// libmcryptEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#define MUTILS_USE_MHASH_CONFIG

#ifdef __cplusplus
extern "C" {
#endif
#include <mhash.h>
#ifdef __cplusplus
}
#endif

#pragma comment(lib, "libmhash.lib")

int main(void)
{
	mutils_word32 i;
	mutils_word32 buf_len;
	MHASH td1, td2, td3;
	const mutils_word8 *buf = (mutils_word8 *) "this is encrpyt sample string!!";
	mutils_word8 *hash1;
	mutils_word8 *hash2;
	hashid alg = MHASH_SHA1;
	mutils_word8 mem[1024];
	mutils_word32 mem_size = sizeof(mem);

	buf_len = mutils_strlen(buf);

	td1 = mhash_init((__hashid)MHASH_SHA1);

	if (td1 == MHASH_FAILED)
	{
		return 0;
	}

	for (i = 0; i < buf_len; ++i)
	{
		mhash(td1, buf + i, 1);
	}

	hash1 = (uint8_t*)mhash_end(td1);

	td2 = mhash_init((__hashid)alg);

	if (td2 == MHASH_FAILED)
	{
		fprintf(stderr, "Failed to init td2.\n");
		exit(MUTILS_INVALID_FUNCTION);
	}

	for (i = 0; i < buf_len / 2; ++i)
	{
		mhash(td2, buf + i, 1);
	}

	if (mhash_save_state_mem(td2, mem, &mem_size) != 0)
	{
		fprintf(stderr, "Error saving state. Size: %d\n", mem_size);
		exit(MUTILS_INVALID_RESULT);
	}

	td3 = mhash_restore_state_mem(mem);

	if (td3 == MHASH_FAILED)
	{
		fprintf(stderr, "Error restoring state.\n");
		exit(MUTILS_INVALID_RESULT);
	}

	for (i = buf_len / 2; i < buf_len; ++i)
	{
		mhash(td3, buf + i, 1);
	}

	hash2 = (uint8_t*)mhash_end(td3);

	if (mutils_memcmp(hash1, hash2, mhash_get_block_size((hashid)alg)) == 0)
	{
		printf("Ok\n");
	}
	else
	{
		printf("Failed\n");
		exit(MUTILS_INVALID_RESULT);
	}

	return 0;
}