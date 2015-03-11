// dlibEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

//������ 20���� ���� ť�� ���� �ִ´�.
	for (int i = 0; i < 20; ++i)
	{
		int a = rand() & 0xFF;

		q.enqueue(a);
	}

	cout << "ť�� �����ϴ� ������ ��� :\n";
	while (q.move_next())
		cout << q.element() << " ";

	cout << "\n\nť�� �������� ������������ ����:\n";
	q.sort();  
	while (q.move_next())
		cout << q.element() << " ";


	cout << "\n\nť���� �������� ���� :\n";
	while (q.size() > 0)
	{
		int a;
		q.dequeue(a);
		cout << a << " ";
	}

	cout << endl;
	return 0;
}

