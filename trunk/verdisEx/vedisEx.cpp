// verdisEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include "vedis.h"

#pragma comment(lib, "vedis.lib")

void Fatal(vedis *pStore, const char *zMsg);
// 키값에 대한 값 요청을 처리하는 정적 콜백 함수
static int DataConsumerCallback(const void *pData, unsigned int nDatalen, void *pUserData /* Unused */);

int main(int argc, char *argv[])
{
	vedis *pStore;   //데이터베이스 핸들
	int rc = VEDIS_OK;

	//데이터베이스를오픈한다. 커맨드 인자로 파일 이름이 있으면 파일 데이터베이스, 그렇지 않으면 메모리 DB로 동작한다.
	rc = vedis_open(&pStore, argc > 1 ? argv[1] : ":mem:" );
	if (rc != VEDIS_OK){
		Fatal(0, "Out of memory");
	}

	//키 'OpenSource' 값 '100'으로 데이터스토어에 저장한다.
	rc = vedis_kv_store(pStore, "OpenSource", -1, "100", 3); 
	if (rc != VEDIS_OK){		
		Fatal(pStore, 0);
	}
	
	//데이터베이스로 부터 키 값에 대한 값을 조회한다.
	//조회결과는 DataConsumerCallback 콜백 함수에서 실행된다.
	rc = vedis_kv_fetch_callback(pStore, "OpenSource", -1, DataConsumerCallback, 0);
	if (rc != VEDIS_OK){	
		Fatal(pStore, 0);
	}
	
//키 'OpenSource'를 데이터스토어에서 지운다.
	vedis_kv_delete(pStore, "OpenSource", -1);

	//자동으로 트랜잭션을 커밋하고 데이터스토어를 닫는다.
	vedis_close(pStore);
	return 0;
}

static int DataConsumerCallback(const void *pData, unsigned int nDatalen, void *pUserData /* Unused */)
{
	BOOL rc;
//키 요청에 대한 데이터값을 콘솔창에 출력한다.
	rc = WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), pData, (DWORD)nDatalen, 0, 0);
	if (!rc){
		return VEDIS_ABORT;
	}

	return VEDIS_OK;
}


void Fatal(vedis *pStore, const char *zMsg)
{
	if (pStore)
	{
		const char *zErr;
		int iLen = 0;

		// 에러 로그를 얻어낸다.
		vedis_config(pStore, VEDIS_CONFIG_ERR_LOG, &zErr, &iLen);
		if (iLen > 0){

			printf("%s", zErr);
		}
	}
	else{
		if (zMsg){
			printf("%s", zMsg);
		}
	}
	
	vedis_lib_shutdown();
	exit(0);
}