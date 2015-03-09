// xmlrpcServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <stdexcept>
#include <iostream>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

using namespace std;

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>


#ifdef _WIN32
#define SLEEP(seconds) SleepEx(seconds * 1000, FALSE);
#else
#define SLEEP(seconds) sleep(seconds);
#endif

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


class sampleAddMethod : public xmlrpc_c::method {
public:
	sampleAddMethod() {
		// signature and help strings are documentation -- the client
		// can query this information with a system.methodSignature and
		// system.methodHelp RPC.
		this->_signature = "i:ii";
		// method's result and two arguments are integers
		this->_help = "This method adds two integers together";
	}
	void
		execute(xmlrpc_c::paramList const& paramList,
		xmlrpc_c::value *   const  retvalP) {

			int const addend(paramList.getInt(0));
			int const adder(paramList.getInt(1));

			paramList.verifyEnd(2);

			*retvalP = xmlrpc_c::value_int(addend + adder);

			// Sometimes, make it look hard (so client can see what it's like
			// to do an RPC that takes a while).
			if (adder == 1)
				SLEEP(2);
		}
};


int _tmain(int argc, _TCHAR* argv[])
{
	try {
		xmlrpc_c::registry myRegistry;

		xmlrpc_c::methodPtr const sampleAddMethodP(new sampleAddMethod);

		myRegistry.addMethod("sample.add", sampleAddMethodP);

		xmlrpc_c::serverAbyss myAbyssServer(
			xmlrpc_c::serverAbyss::constrOpt()
			.registryP(&myRegistry)
			.portNumber(8080));

		myAbyssServer.run();
		// xmlrpc_c::serverAbyss.run() never returns
		assert(false);
	}
	catch (exception const& e) {
		cerr << "Something failed.  " << e.what() << endl;
	}
	return 0;
}