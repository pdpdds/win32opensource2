// entityxEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "entityx/entityx.h"

using namespace entityx;

#ifdef _DEBUG
#pragma comment(lib, "entityx-d.lib")
#else
#pragma comment(lib, "entityx.lib")
#endif

struct Position {
	Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	float x, y;
};

struct Direction {
	Direction(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	float x, y;
};

int _tmain(int argc, _TCHAR* argv[])
{
	EntityX entityx;

	entityx::Entity entity = entityx.entities.create();
	entity.assign<Position>(1.0f, 2.0f);
	entity.assign<Direction>(1.0f, 1.0f);

	ComponentHandle<Position> position;
	ComponentHandle<Direction> direction;

	for (Entity entity : entityx.entities.entities_with_components(position, direction)) {
		ComponentHandle<Position> position = entity.component<Position>();
		if (position) {							
			printf("%f %f\n", position.get()->x, position.get()->y);
		}
	}

	entity.destroy();

	return 0;
}

