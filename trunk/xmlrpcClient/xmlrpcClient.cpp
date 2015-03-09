// xmlrpcClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#pragma comment(lib, "libxmlrpc++.lib")
#pragma comment(lib, "libxmlrpc_xmltok.lib")
#pragma comment(lib, "libxmlrpc_xmlparse.lib")
#pragma comment(lib, "libxmlrpc_util.lib")
#pragma comment(lib, "libxmlrpc_server_w32httpsys.lib")
#pragma comment(lib, "libxmlrpc.lib")
#pragma comment(lib, "libxmlrpc_abyss.lib")
#pragma comment(lib, "libxmlrpc_server.lib")
#pragma comment(lib, "libxmlrpc_server_abyss.lib")
#pragma comment(lib, "libxmlrpc_server_cgi.lib")
#pragma comment(lib, "libxmlrpc_client.lib")
#pragma comment(lib, "Wininet.lib")

int _tmain(int argc, _TCHAR* argv[])
{

	if (argc - 1 > 0) {
		cerr << "This program has no arguments" << endl;
		exit(1);
	}

	try {
		string const serverUrl("http://localhost:8080/RPC2");
		string const methodName("sample.add");

		xmlrpc_c::clientSimple myClient;
		xmlrpc_c::value result;

		myClient.call(serverUrl, methodName, "ii", &result, 5, 7);

		int const sum = xmlrpc_c::value_int(result);
		// Assume the method returned an integer; throws error if not

		cout << "Result of RPC (sum of 5 and 7): " << sum << endl;

	}
	catch (exception const& e) {
		cerr << "Client threw error: " << e.what() << endl;
	}
	catch (...) {
		cerr << "Client threw unexpected error." << endl;
	}

	getchar();

	return 0;
}
