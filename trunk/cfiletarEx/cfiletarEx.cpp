// cfiletarEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "FileTar.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CFileTar ft;
//tar ���� ���� ��� �߰�
	ft.SetHeaderDescription("text file archive");

	char szCurPath[MAX_PATH] = { 0, };
	char* szTarName = "txt.tar";
	GetCurrentDirectoryA(MAX_PATH, szCurPath);
	
//���� ���� ��� ����
	ft.SetFilePath(szCurPath);
//CFileTar ��ü�� ������ ���ϵ��� �߰��Ѵ�.
	ft.AddFile("ReadMe.txt", "Readme File");
	ft.AddFile("filetar.h", "CFileTar Header");
	ft.AddFile("filetar.cpp", "CFileTar Body");

//���������� �����Ѵ�.
	ft.CreateTar(szTarName);

//�������� �̸��� ������ ��ü ��� ��Ʈ���� �����. 
	CFileTar::TarHeader t;
	std::string tarFullPathName = szCurPath;
	tarFullPathName += "\\";
	tarFullPathName += szTarName;

//tar ���� ���Ϸ� ���� ���ϼ� �� ������ ����� ���� ���� ������ ����
	CFileTar::GetTarInfo((char*)tarFullPathName.c_str(), &t);
	cout << "Number of archived files : " << t.GetCount() << "\r\n";
	cout << "Description of tar file : " << t.GetDescription() << "\r\n";

	
//��������
	//CFileTar::UnTar((char*)tarFullPathName.c_str(), szCurPath);

	return 0;
}

