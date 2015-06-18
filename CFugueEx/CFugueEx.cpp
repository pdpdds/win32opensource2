// CFugueEx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CFugueLib.h"

#pragma comment(lib, "WinMM.lib")
#ifdef _DEBUG
#pragma comment(lib, "CFuguedu.lib")
#pragma comment(lib, "jdkmididu.lib")
#else
#pragma comment(lib, "CFugueu.lib")
#pragma comment(lib, "jdkmidiu.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	CFugue::PlayMusicString(_T("K[MELA_DEFAULT] S R G M P D N"));
	return 0;
}

