// glmEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

typedef struct tag_Triangle
{
	glm::vec3 position[3];	
	glm::vec3 normal;

}Triangle;

void computeNormal(Triangle & triangle)
{
	glm::vec3 const & a = triangle.position[0];
	glm::vec3 const & b = triangle.position[1];
	glm::vec3 const & c = triangle.position[2];
	triangle.normal = glm::normalize(glm::cross(c - a, b - a));
}



int _tmain(int argc, _TCHAR* argv[])
{
	Triangle triangle;
	triangle.position[0] = glm::vec3(0, 0, 0);
	triangle.position[1] = glm::vec3(1, 1, 0);
	triangle.position[2] = glm::vec3(2, 0, 0);
	computeNormal(triangle);

	printf("normal vector is (%f,%f,%f)\n", triangle.normal.x, triangle.normal.y, triangle.normal.z);

	return 0;
}

