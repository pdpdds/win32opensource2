// coldetEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "coldet.h"

#pragma comment(lib, "libcoldet.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	CollisionModel3D* model = newCollisionModel3D();
	float vertex1[3] = { 0, 0, 0};
	float vertex2[3] = { 0, 1, 0};
	float vertex3[3] = { 0, 1, 1};

	model->addTriangle(vertex1, vertex2, vertex3);
	model->finalize();

	CollisionModel3D* model2 = newCollisionModel3D();
	float vertex4[3] = { 0, 0.5, 0 };
	float vertex5[3] = { 0, 1.5, 0 };
	float vertex6[3] = { 0, 1.5, 1.5 };

	model2->addTriangle(vertex4, vertex5, vertex6);
	model2->finalize();

	if (model->collision(model2))
	{
		printf("collison detection!!\n");
	}

	return 0;
}

