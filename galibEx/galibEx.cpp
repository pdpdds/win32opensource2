// galibEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <ga/GASimpleGA.h>	// we're going to use the simple GA
#include <ga/GA2DBinStrGenome.h> // and the 2D binary string genome
#include <ga/std_stream.h>

#define cout STD_COUT

#pragma comment(lib, "ga.lib")

float Objective(GAGenome &);	//유전자의 진화 전략

int
main(int argc, char **argv)
{
	cout << "Example 1\n\n";
	cout << "This program tries to fill a 2DBinaryStringGenome with\n";
	cout << "alternating 1s and 0s using a SimpleGA\n\n"; cout.flush();

	int popsize = 30; //유전정보 크기
	int ngen = 400; //400세대
	float pmut = 0.001; //돌연변이 확률
	float pcross = 0.9; //교차 혼합 확률

//가로 10, 세로 5의 이진 문자열 유전자를 선언하고 앞에서 선언한 진화전략 함수를 넣어준다.	
	int width = 10;
	int height = 5;
	GA2DBinaryStringGenome genome(width, height, Objective);

	//유전자를 가지고 이 유전자가 어떻게 진화할지를 정의하는 유전 알고리즘을 선언한다.
	//유전 알고리즘의 파라메터로는 돌연변이 확률, 세대를 거친 수, 교차 혼합 확률등이 들어간다.
	GASimpleGA ga(genome); //galib 라이브러리에서 제공하는 심플 유전 알고리즘 선언
	ga.populationSize(popsize); //유전정보의 크기를 정의한다.
	ga.nGenerations(ngen); //몇세대를 걸쳐 진화할 것인가?
	ga.pMutation(pmut); //돌연변이 확률
	ga.pCrossover(pcross); //교차 혼합 확률

	//이렇게 유전자와 유전 알고리즘을 정의하고 게놈의 진화를 시작한다.
	ga.evolve();

//진화 결과 최종적인 게놈 정보를 얻는다.	
	cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";

	return 0;
}

//게놈의 진화 전략
//이진 문자열 값을 확인해서 최종 score 값을 리턴한다.
//score값에 따라 앞에서 선언한 진화 알고리즘이 이 값을 참조하여 
//이진 문자열 게놈(유전 정보)를 변이시킨다.
float Objective(GAGenome& g) {
	GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)g;
	float score = 0.0;
	int count = 0;
	for (int i = 0; i<genome.width(); i++){
		for (int j = 0; j<genome.height(); j++){
			if (genome.gene(i, j) == 0 && count % 2 == 0)
				score += 1.0;
			if (genome.gene(i, j) == 1 && count % 2 != 0)
				score += 1.0;
			count++;
		}
	}
	return score;
}


