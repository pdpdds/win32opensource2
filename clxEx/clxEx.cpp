// clxEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "clx/tokenizer.h"

int main(int argc, char* argv[]) {
	std::string s = "This is, a test";
	clx::tokenizer tok(s);

	for (unsigned int i = 0; i < tok.size(); i++) {
		std::cout << '<' << tok.at(i) << "> ";
	}
	std::cout << std::endl;

	return 0;
}