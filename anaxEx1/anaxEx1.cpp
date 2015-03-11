// anaxEx1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "anax/anax.hpp"
#include "anax/Entity.hpp"
#include "anax/Component.hpp"
#include <iostream>

#pragma comment(lib, "anax_d.lib")

using namespace anax;

class PositionComponent : public anax::Component<PositionComponent>
{
public:
	float x, y, z;
	PositionComponent(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class VelocityComponent : public anax::Component<VelocityComponent>
{
public:
	float x, y, z;
	VelocityComponent(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class MovementSystem : public anax::System<MovementSystem>
{
public:

	MovementSystem()
		: Base(anax::ComponentFilter().requires<PositionComponent, VelocityComponent>())
	{
	}

	void update()
	{
		auto entities = getEntities();
		for (auto& e : entities)
		{
			auto position = e.getComponent<PositionComponent>();
			auto velocity = e.getComponent<VelocityComponent>();

			position.x += velocity.x;
			position.y += velocity.y;
			position.z += velocity.z;
		}
	}

private:

	virtual void onEntityAdded(anax::Entity& e) override
	{
		std::cout << "Entity added to MovementSystem\n";
	}

	virtual void onEntityRemoved(anax::Entity& e) override
	{
		std::cout << "Entity removed from MovementSystem\n";
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	World* world = new World();
	MovementSystem moveSystem;
	world->addSystem(moveSystem);

	Entity entity1 = world->createEntity();

	entity1.addComponent<PositionComponent>(2.0f, 3.0f, 5.0f);		
	entity1.addComponent<VelocityComponent>(2.0f, 3.0f, -5.0f);
	entity1.activate();
		
	world->refresh();

	moveSystem.update();

	return 0;
}

