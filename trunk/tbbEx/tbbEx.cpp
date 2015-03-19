// tbbEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "tbb/parallel_for_each.h"
#include "tbb/task_scheduler_init.h"
#include <iostream>
#include <vector>

#define sumCountPerTask 100000

struct AddTask {
//���� �۾����� ����� �Լ� ��ü. ���۰����� ���۰� + sumCountPerTask���� ���� �ջ��Ѵ�.
	void operator()() {
		for (_int64 i = _startNum; i < _startNum + sumCountPerTask; ++i)
		{
			_sum += i;
		}

	}

	AddTask(_int64 startNum)
	{
		_startNum = startNum;
		_sum = 0;
	}	

	_int64 _sum;
	_int64 _startNum;
};

template <typename T> struct invoker {
	void operator()(T& it) const { it(); }
};

//1���� 1000000000���� �ջ��ϴ� ���α׷� 
//sumCountPerTask ��ŭ�� �½�ũ�� �Ҵ��ϰ� �½�ũ�� �Ҵ�� ������ ���� ������ ����� �����Ѵ�.
int main(int, char**) {


	//���� �����췯 �ʱ�ȭ ��������� ������ ���� ����
	tbb::task_scheduler_init init(10);

	//���� �迭�� 100���� �½�ũ�� �߰��Ѵ�.
	std::vector<AddTask> tasks;
	_int64 value = 1;
	for (_int64 i = 0; i < 10000; ++i)
	{

		tasks.push_back(AddTask(value));
		value += sumCountPerTask;
	}

	//���ķ� 100���� �½�ũ�� �����Ѵ�.
	//����ü���� ������ �Լ��ڰ� ����ȴ�.
	tbb::parallel_for_each(tasks.begin(), tasks.end(), invoker<AddTask>());

	//������ �½�ũ�� ���� ����� ���Ѵ�.
	_int64 result = 0;
	for (auto task : tasks)
	{
		result += task._sum;
	}

	printf("1���� %I64u���� �ջ��� ��� : %I64u", value - 1, result);

	return 0;
}


/*
int main(int, char**) {

	_int64 result = 0;
	for (_int64 i = 1; i <= 1000000000; i++)
	{
		result += i;
	}

	printf("1���� 1000000000 ���� �ջ��� ��� : %I64u", result);

	return 0;
}
*/


