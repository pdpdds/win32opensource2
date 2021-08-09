#include <Windows.h>
#include "FileTar.h"
#include <iostream>
#include <tchar.h>

using namespace std;

//���ϵ��� ����
void Archive(char* szCurPath, char* fileName)
{
	CFileTar ft;
	//tar ���� ���� ��� �߰�
	ft.SetHeaderDescription("text file archive");

	//���� ���� ��� ����
	ft.SetFilePath(szCurPath);
	char* szTarName = fileName;
	
	//CFileTar ��ü�� ������ ���ϵ��� �߰��Ѵ�.
	ft.AddFile("ReadMe.txt", "Readme File");
	ft.AddFile("filetar.h", "CFileTar Header");
	ft.AddFile("filetar.cpp", "CFileTar Body");

	//���������� �����Ѵ�.
	ft.CreateTar(szTarName);
}

//������ Ǭ��.
void Extract(char* szCurPath, char* fileName)
{
	//�������� �̸��� ������ ��ü ��� ��Ʈ���� �����. 
	CFileTar::TarHeader t;
	std::string tarFullPathName = szCurPath;
	tarFullPathName += "\\";
	tarFullPathName += fileName;

	//tar ���� ���Ϸ� ���� ���ϼ� �� ������ ����� ���� ���� ������ ����
	CFileTar::GetTarInfo((char*)tarFullPathName.c_str(), &t);
	cout << "Number of archived files : " << t.GetCount() << "\r\n";
	cout << "Description of tar file : " << t.GetDescription() << "\r\n";


	//��������
	strcat(szCurPath, "\\test");
	CFileTar::UnTar((char*)tarFullPathName.c_str(), szCurPath);
}

//CFileTar ����� ���� �׽�Ʈ
//������Ʈ ������ test ������ ������ ������ ���������� Ǯ��
int _tmain(int argc, _TCHAR* argv[])
{
	char szCurPath[MAX_PATH] = { 0, };
	char* filename = "test.tar";
	GetCurrentDirectoryA(MAX_PATH, szCurPath);

	Archive(szCurPath, filename);
	Extract(szCurPath, filename);

	return 0;
}

