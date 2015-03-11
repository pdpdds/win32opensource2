// rubyEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <ruby.h>

using namespace std;

#pragma comment(lib, "msvcr120-ruby200.lib")

typedef VALUE(ruby_method)(...);

int main(int argc, char **argv)
{
	int status;

	//루비 초기화
	ruby_sysinit(&argc, &argv);
	RUBY_INIT_STACK;
	ruby_init();
	ruby_init_loadpath();

	// test.rb 스크립트 로드
	rb_load_protect(rb_str_new2("./test.rb"), 0, &status);

	if (status) 
	{
//루비 스크립트 실행
		VALUE rbError = rb_funcall(rb_gv_get("$!"), rb_intern("message"), 0);
		cerr << StringValuePtr(rbError) << endl;
	};

//루비 엔진 종료
	ruby_finalize();

	return status;
}