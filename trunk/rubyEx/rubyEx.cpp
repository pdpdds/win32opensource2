// rubyEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	//��� �ʱ�ȭ
	ruby_sysinit(&argc, &argv);
	RUBY_INIT_STACK;
	ruby_init();
	ruby_init_loadpath();

	// test.rb ��ũ��Ʈ �ε�
	rb_load_protect(rb_str_new2("./test.rb"), 0, &status);

	if (status) 
	{
//��� ��ũ��Ʈ ����
		VALUE rbError = rb_funcall(rb_gv_get("$!"), rb_intern("message"), 0);
		cerr << StringValuePtr(rbError) << endl;
	};

//��� ���� ����
	ruby_finalize();

	return status;
}