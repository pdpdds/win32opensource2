#include <Windows.h>
#include "FileTar.h"
#include <iostream>
#include <tchar.h>

using namespace std;

//파일들을 압축
void Archive(char* szCurPath, char* fileName)
{
	CFileTar ft;
	//tar 파일 설명 헤더 추가
	ft.SetHeaderDescription("text file archive");

	//현재 폴더 경로 설정
	ft.SetFilePath(szCurPath);
	char* szTarName = fileName;
	
	//CFileTar 객체에 압축할 파일들을 추가한다.
	ft.AddFile("ReadMe.txt", "Readme File");
	ft.AddFile("filetar.h", "CFileTar Header");
	ft.AddFile("filetar.cpp", "CFileTar Body");

	//압축파일을 생성한다.
	ft.CreateTar(szTarName);
}

//압축을 푼다.
void Extract(char* szCurPath, char* fileName)
{
	//압축파일 이름을 포함한 전체 경로 스트링을 만든다. 
	CFileTar::TarHeader t;
	std::string tarFullPathName = szCurPath;
	tarFullPathName += "\\";
	tarFullPathName += fileName;

	//tar 압축 파일로 부터 파일수 및 파일의 헤더에 대한 설명 데이터 추출
	CFileTar::GetTarInfo((char*)tarFullPathName.c_str(), &t);
	cout << "Number of archived files : " << t.GetCount() << "\r\n";
	cout << "Description of tar file : " << t.GetDescription() << "\r\n";


	//압축해제
	strcat(szCurPath, "\\test");
	CFileTar::UnTar((char*)tarFullPathName.c_str(), szCurPath);
}

//CFileTar 압축과 해제 테스트
//프로젝트 폴더에 test 폴더를 만들어야 압축이 정상적으로 풀림
int _tmain(int argc, _TCHAR* argv[])
{
	char szCurPath[MAX_PATH] = { 0, };
	char* filename = "test.tar";
	GetCurrentDirectoryA(MAX_PATH, szCurPath);

	Archive(szCurPath, filename);
	Extract(szCurPath, filename);

	return 0;
}

