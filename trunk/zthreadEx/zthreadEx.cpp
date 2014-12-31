// zthreadEx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zthread/Thread.h"
#include <iostream>

#pragma comment(lib, "zthread.lib")

using namespace ZThread;

class aRunnable : public Runnable {
public:

	void run() {

		Thread::sleep(1000);
		std::cout << "Hello from another thread" << std::endl;

	}

};

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		
		Thread t(new aRunnable);
		t.wait();

	}
	catch (Synchronization_Exception& e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "Hello from the main thread" << std::endl;

	getchar();

	return 0;
}