// entityxEx2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "entityx/entityx.h"
#include "entityx/deps/Dependencies.h"

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

struct MovementSystem : public System<MovementSystem> {
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		ComponentHandle<Position> position;
		ComponentHandle<Direction> direction;
		for (Entity entity : es.entities_with_components(position, direction)) {
			position->x += direction->x * dt;
			position->y += direction->y * dt;
		}
	};
};

int _tmain(int argc, _TCHAR* argv[])
{
	EntityX entityx;
	entityx.systems.add<MovementSystem>();
	entityx.systems.configure();

	entityx::Entity entity = entityx.entities.create();
	entity.assign<Position>(1.0f, 2.0f);
	entity.assign<Direction>(1.0f, 1.0f);

	float dt = 0.1f; //샘플 값. 아래 함수는 게임 루프에서 호출되어야 한다.
	entityx.systems.update<MovementSystem>(dt);

	entity.destroy();

	return 0;
}

