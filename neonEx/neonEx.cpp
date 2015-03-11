// neonEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include"WebdavClient.h"

#ifdef _DEBUG
#pragma comment(lib, "libneonD.lib")
#else
#pragma comment(lib, "libneon.lib")
#endif

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libexpat.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h>
#include <neon/ne_request.h> 
#include <neon/ne_auth.h>
#include "neon/ne_basic.h" /* ne_session, ne_put, ne_get, ne_delete, ne_mkcol */
#include"unistd.h"

int _tmain(int argc, _TCHAR* argv[])
{
	

	const std::string host("http://211.47.85.248:18080/webdav/");
	const std::string user("darkx3");
	const std::string pass("genius!3456");
	const std::string uri("darkx3");
	const unsigned port = 18000;

	WebdavClient webdavClient(host, port, user, pass);

	// Upload a file to webdav server
	if (webdavClient.put(uri + "config.xml", "config.xml")){
		std::cout << "Upload succesfull!" << std::endl;
	}
	else {
		std::cout << "Upload error! " << webdavClient.getLastError() << std::endl;
	}

	// Download a file from webdav server
	if (webdavClient.get(uri + "my_file_put", "my_file_get")){
		std::cout << "Download successfull!" << std::endl;
	}
	else {
		std::cout << "Download error! " << webdavClient.getLastError() << std::endl;
	}
	
	// Check if a file on webdav server exists
	if (webdavClient.exist(uri + "my_file_put")){
		std::cout << "File exists! " << std::endl;
	}
	else {
		std::cout << "File does not exist! " << webdavClient.getLastError() << std::endl;
	}

	// Create directory on webdav server
	if (webdavClient.mkdir(uri + "new_dir")){
		std::cout << "Create folder successfull!" << std::endl;
	}
	else {
		std::cout << "Create folder error! " << webdavClient.getLastError() << std::endl;
	}

	// Remove a file on webdav server
	if (webdavClient.rm(uri + "my_file_put")){
		std::cout << "Remove file successfull!" << std::endl;
	}
	else {
		std::cout << "Remove file error! " << webdavClient.getLastError() << std::endl;
	}

	// Remove a directory on webdav server
	if (webdavClient.rm(uri + "new_dir/")){
		std::cout << "Remove directory successfull!" << std::endl;
	}
	else {
		std::cout << "Remove directory error! " << webdavClient.getLastError() << std::endl;
	}
	
	return 0;
}

