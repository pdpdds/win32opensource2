// lemonParserEx1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <malloc.h>
#include "example1.h"

extern void *ParseAlloc(void *(*mallocProc)(size_t));
extern void Parse(
	void *yyp,                   /* The parser */
	int yymajor,                 /* The major token code number */
	ParseTOKENTYPE yyminor       /* The value for the token */
	ParseARG_PDECL               /* Optional %extra_argument parameter */
	);

int _tmain(int argc, _TCHAR* argv[])
{
	void* pParser = ParseAlloc(malloc);

	Parse(pParser, INTEGER, 15);
	Parse(pParser, DIVIDE, 0);
	Parse(pParser, INTEGER, 5);
	Parse(pParser, 0, 0);
	return 0;
}

