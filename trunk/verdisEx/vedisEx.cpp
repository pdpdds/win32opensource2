// verdisEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Windows.h>
#include "vedis.h"

#pragma comment(lib, "vedis.lib")

void Fatal(vedis *pStore, const char *zMsg);
// Ű���� ���� �� ��û�� ó���ϴ� ���� �ݹ� �Լ�
static int DataConsumerCallback(const void *pData, unsigned int nDatalen, void *pUserData /* Unused */);

int main(int argc, char *argv[])
{
	vedis *pStore;   //�����ͺ��̽� �ڵ�
	int rc = VEDIS_OK;

	//�����ͺ��̽��������Ѵ�. Ŀ�ǵ� ���ڷ� ���� �̸��� ������ ���� �����ͺ��̽�, �׷��� ������ �޸� DB�� �����Ѵ�.
	rc = vedis_open(&pStore, argc > 1 ? argv[1] : ":mem:" );
	if (rc != VEDIS_OK){
		Fatal(0, "Out of memory");
	}

	//Ű 'OpenSource' �� '100'���� �����ͽ��� �����Ѵ�.
	rc = vedis_kv_store(pStore, "OpenSource", -1, "100", 3); 
	if (rc != VEDIS_OK){		
		Fatal(pStore, 0);
	}
	
	//�����ͺ��̽��� ���� Ű ���� ���� ���� ��ȸ�Ѵ�.
	//��ȸ����� DataConsumerCallback �ݹ� �Լ����� ����ȴ�.
	rc = vedis_kv_fetch_callback(pStore, "OpenSource", -1, DataConsumerCallback, 0);
	if (rc != VEDIS_OK){	
		Fatal(pStore, 0);
	}
	
//Ű 'OpenSource'�� �����ͽ����� �����.
	vedis_kv_delete(pStore, "OpenSource", -1);

	//�ڵ����� Ʈ������� Ŀ���ϰ� �����ͽ��� �ݴ´�.
	vedis_close(pStore);
	return 0;
}

static int DataConsumerCallback(const void *pData, unsigned int nDatalen, void *pUserData /* Unused */)
{
	BOOL rc;
//Ű ��û�� ���� �����Ͱ��� �ܼ�â�� ����Ѵ�.
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

		// ���� �α׸� ����.
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