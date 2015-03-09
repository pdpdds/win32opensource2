// libashttpEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "header.hpp"
#include "client/client.hpp"
#include "client/request.hpp"
#include "type.hpp"

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <iostream>
#include <thread>

#pragma comment(lib, "libashttp.lib")
#pragma comment(lib, "templog.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

using namespace ashttp;
using namespace ashttp::client;

ashttp::asio::io_service* pIoService;

void sigintHandler(int);

int _tmain(int argc, _TCHAR* argv[])
{
	boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::severity_level::trace);

	ashttp::asio::io_service ioService;
	pIoService = &ioService;

	signal(SIGTERM, sigintHandler);

	{
		const auto client = ClientHTTPS::create("www.google.co.kr", ioService);

		client->onConnect([](const ErrorCode& ec) {
			std::cerr << "\tclient onconnect " << ec << std::endl;
		});

		{
			auto request = client->get("/a");

			request->onBodyChunk([](const ErrorCode& ec, std::istream& is, std::size_t chunkSize) {
				std::cerr << "\trequest body chunk; size: " << chunkSize << std::endl;

				is.ignore(chunkSize);
			})
				.onComplete([client, request](const ErrorCode& ec) {
				std::cerr << "\trequest oncomplete" << ec << std::endl;
			});

			client->schedule(request);
		}

	}

	std::thread t{ [&ioService]() { ioService.run(); } };

	std::cerr << "\tJoining io_service thread." << std::endl;

	t.join();

	std::cerr << "\tJoined io_service thread." << std::endl;

	return 0;
}

void sigintHandler(int) {
	std::cerr << "\tClose requested." << std::endl;

	pIoService->stop();
}