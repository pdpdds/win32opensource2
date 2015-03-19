// tbbEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "tbb/parallel_for_each.h"
#include "tbb/task_scheduler_init.h"
#include <iostream>
#include <vector>

#define sumCountPerTask 100000

struct AddTask {
//병렬 작업에서 실행될 함수 객체. 시작값에서 시작값 + sumCountPerTask까지 수를 합산한다.
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

//1부터 1000000000까지 합산하는 프로그램 
//sumCountPerTask 만큼을 태스크에 할당하고 태스크는 할당된 범위의 수를 더한후 결과를 저장한다.
int main(int, char**) {


	//병렬 스케쥴러 초기화 명시적으로 스레드 수를 설정
	tbb::task_scheduler_init init(10);

	//벡터 배열에 100개의 태스크를 추가한다.
	std::vector<AddTask> tasks;
	_int64 value = 1;
	for (_int64 i = 0; i < 10000; ++i)
	{

		tasks.push_back(AddTask(value));
		value += sumCountPerTask;
	}

	//병렬로 100개의 태스크를 실행한다.
	//구조체에서 정의한 함수자가 실행된다.
	tbb::parallel_for_each(tasks.begin(), tasks.end(), invoker<AddTask>());

	//각각의 태스크가 구한 결과를 더한다.
	_int64 result = 0;
	for (auto task : tasks)
	{
		result += task._sum;
	}

	printf("1부터 %I64u까지 합산한 결과 : %I64u", value - 1, result);

	return 0;
}


/*
int main(int, char**) {

	_int64 result = 0;
	for (_int64 i = 1; i <= 1000000000; i++)
	{
		result += i;
	}

	printf("1부터 1000000000 까지 합산한 결과 : %I64u", result);

	return 0;
}
*/


