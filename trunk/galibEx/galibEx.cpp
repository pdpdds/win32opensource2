// galibEx.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <ga/GASimpleGA.h>	// we're going to use the simple GA
#include <ga/GA2DBinStrGenome.h> // and the 2D binary string genome
#include <ga/std_stream.h>

#define cout STD_COUT

#pragma comment(lib, "ga.lib")

float Objective(GAGenome &);	//�������� ��ȭ ����

int
main(int argc, char **argv)
{
	cout << "Example 1\n\n";
	cout << "This program tries to fill a 2DBinaryStringGenome with\n";
	cout << "alternating 1s and 0s using a SimpleGA\n\n"; cout.flush();

	int popsize = 30; //�������� ũ��
	int ngen = 400; //400����
	float pmut = 0.001; //�������� Ȯ��
	float pcross = 0.9; //���� ȥ�� Ȯ��

//���� 10, ���� 5�� ���� ���ڿ� �����ڸ� �����ϰ� �տ��� ������ ��ȭ���� �Լ��� �־��ش�.	
	int width = 10;
	int height = 5;
	GA2DBinaryStringGenome genome(width, height, Objective);

	//�����ڸ� ������ �� �����ڰ� ��� ��ȭ������ �����ϴ� ���� �˰����� �����Ѵ�.
	//���� �˰����� �Ķ���ͷδ� �������� Ȯ��, ���븦 ��ģ ��, ���� ȥ�� Ȯ������ ����.
	GASimpleGA ga(genome); //galib ���̺귯������ �����ϴ� ���� ���� �˰��� ����
	ga.populationSize(popsize); //���������� ũ�⸦ �����Ѵ�.
	ga.nGenerations(ngen); //��븦 ���� ��ȭ�� ���ΰ�?
	ga.pMutation(pmut); //�������� Ȯ��
	ga.pCrossover(pcross); //���� ȥ�� Ȯ��

	//�̷��� �����ڿ� ���� �˰����� �����ϰ� �Գ��� ��ȭ�� �����Ѵ�.
	ga.evolve();

//��ȭ ��� �������� �Գ� ������ ��´�.	
	cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";

	return 0;
}

//�Գ��� ��ȭ ����
//���� ���ڿ� ���� Ȯ���ؼ� ���� score ���� �����Ѵ�.
//score���� ���� �տ��� ������ ��ȭ �˰����� �� ���� �����Ͽ� 
//���� ���ڿ� �Գ�(���� ����)�� ���̽�Ų��.
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


