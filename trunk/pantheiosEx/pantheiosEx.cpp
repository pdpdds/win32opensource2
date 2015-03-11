// pantheiosEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "pantheios/pantheios.hpp" //primary header file, always be included
#include "pantheios/frontends/stock.h"

//Specify process identity
const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = PANTHEIOS_LITERAL_STRING("pantheiosEx.exe");

#pragma comment(lib, "pantheios.1.util.vc12.dll.debug.lib")
#pragma comment(lib, "pantheios.1.core.vc12.dll.debug.lib")
#pragma comment(lib, "pantheios.1.fe.simple.vc12.dll.debug.lib")
#pragma comment(lib, "pantheios.1.be.fprintf.vc12.dll.debug.lib")
#pragma comment(lib, "pantheios.1.bec.fprintf.vc12.dll.debug.lib")

int main(int argc, char** argv){

	try
	{
		pantheios::log_DEBUG(PANTHEIOS_LITERAL_STRING("debug"));
		pantheios::log_INFORMATIONAL(PANTHEIOS_LITERAL_STRING("informational"));
		pantheios::log_NOTICE(PANTHEIOS_LITERAL_STRING("notice"));
		pantheios::log_WARNING(PANTHEIOS_LITERAL_STRING("warning"));
		pantheios::log_ERROR(PANTHEIOS_LITERAL_STRING("error"));
		pantheios::log_CRITICAL(PANTHEIOS_LITERAL_STRING("critical"));
		pantheios::log_ALERT(PANTHEIOS_LITERAL_STRING("alert"));
		pantheios::log_EMERGENCY(PANTHEIOS_LITERAL_STRING("emergency"));
	}
	catch (std::bad_alloc&){
		pantheios::log_ALERT(PANTHEIOS_LITERAL_STRING("out of memory"));
	}
	catch (std::exception& x){
		pantheios::log_CRITICAL(PANTHEIOS_LITERAL_STRING("Exception: "), x);
	}
	catch (...){
		pantheios::logputs(pantheios::emergency, PANTHEIOS_LITERAL_STRING("Unknown error"));
	}

	return 0;
}