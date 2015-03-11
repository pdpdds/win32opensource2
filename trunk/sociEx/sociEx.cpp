// sociEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


#include "soci.h"
#include "mysql/soci-mysql.h"
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include <cmath>

#pragma comment(lib, "libsoci_core_3_2.lib")
#pragma comment(lib, "libsoci_mysql_3_2.lib")
#pragma comment(lib, "libmysql.lib")

using namespace soci;

int main(int argc, char** argv)
{

	session sql(mysql, "db=test user=root password=root");

	int count;
	sql << "select count(*) from tbllogin;", into(count);

	return 0;
}