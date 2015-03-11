// dlibEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <dlib/queue.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace dlib;

typedef dlib::queue<int>::sort_1b_c queue_of_int;

int _tmain(int argc, _TCHAR* argv[])
{
	queue_of_int q;

	srand(time(0));

//랜덤한 20개의 값을 큐에 집어 넣는다.
	for (int i = 0; i < 20; ++i)
	{
		int a = rand() & 0xFF;

		q.enqueue(a);
	}

	cout << "큐에 존재하는 아이템 출력 :\n";
	while (q.move_next())
		cout << q.element() << " ";

	cout << "\n\n큐의 아이템을 오름차순으로 소팅:\n";
	q.sort();  
	while (q.move_next())
		cout << q.element() << " ";


	cout << "\n\n큐에서 아이템을 제거 :\n";
	while (q.size() > 0)
	{
		int a;
		q.dequeue(a);
		cout << a << " ";
	}

	cout << endl;
	return 0;
}

