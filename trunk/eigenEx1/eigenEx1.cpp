// eigenEx1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


void primitiveDeclaration();
void matrixOperation();

int _tmain(int argc, _TCHAR* argv[])
{
	primitiveDeclaration();
	matrixOperation();

	return 0;
}

void primitiveDeclaration()
{
	Matrix3f m3;
	m3 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
	Matrix4f m4 = Matrix4f::Identity();
	Vector4i v4(1, 2, 3, 4);

	std::cout << "m3\n" << m3 << "\nm4:\n"
		<< m4 << "\nv4:\n" << v4 << std::endl;
}

void matrixOperation()
{
	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;
}
