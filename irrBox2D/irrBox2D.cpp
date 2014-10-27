// irrBox2D.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#define _IRR_STATIC_LIB_
#include <irrlicht.h>
#include <Box2D/Box2D.h>

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "box2d.lib")

#define WIDTH 640
#define HEIGHT 480

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const float M2P = 20;
const float P2M = 1 / M2P;
const video::SColor white = video::SColor(255, 255, 255, 255);
b2World* world;
IVideoDriver* driver;
bool execute = 1;

class MyEventReceiver : public IEventReceiver
{
public:
	struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		SMouseState() : LeftButtonDown(0) { }
	} MouseState;

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch (event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				MouseState.LeftButtonDown = 1;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				MouseState.LeftButtonDown = 0;
				break;
			case EMIE_MOUSE_MOVED:
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				break;
			default:
				break;
			}
		}
		else if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			switch (event.KeyInput.Key)
			{
			case KEY_ESCAPE:
				execute = 0;
				break;
			default:
				break;
			}
		}
		return 0;
	}

	const SMouseState& GetMouseState(void) const
	{
		return MouseState;
	}


	MyEventReceiver()
	{
	}
};

b2Body* addRect(int x, int y, int w, int h, b2BodyType dyn = b2_dynamicBody)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	//bodydef.fixedRotation=1;
	bodydef.type = dyn;
	b2Body* body = world->CreateBody(&bodydef);
	b2PolygonShape shape;
	shape.SetAsBox(P2M*w / 2, P2M*h / 2);
	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	body->CreateFixture(&fixturedef);
	return body;
}

inline void drawWorld()
{
	b2Body* tmp = world->GetBodyList();
	b2Vec2 points[4];
	driver->beginScene(1, 1, SColor(255, 77, 77, 77));
	while (tmp)
	{
#define point(i) ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i)
		const core::vector2di centerLoc(tmp->GetWorldCenter().x*M2P, tmp->GetWorldCenter().y*M2P);
		core::vector2di locVec[4];
		for (u32 i = 0; i<4; i++)
		{
			locVec[i] = core::vector2di(centerLoc.X + point(i).x*M2P, centerLoc.Y + point(i).y*M2P);
			//std::cout<<"locVec["<<i<<"]="<<locVec[i].X<<","<<locVec[i].Y<<std::endl;//generate some lag
			locVec[i].rotateBy(tmp->GetAngle()*180.0 / 3.14159265358979323846, centerLoc);
		}
		for (u32 i = 0; i<3; i++)
			driver->draw2DLine(locVec[i], locVec[i + 1], white);
		driver->draw2DLine(locVec[3], locVec[0], white);

		tmp = tmp->GetNext();
	}
	driver->endScene();
}

int main()
{
	MyEventReceiver receiver;
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, dimension2d<u32>(WIDTH, HEIGHT), 16, 0, 0, 1, &receiver);
	if (!device) return 1;
	driver = device->getVideoDriver();
	world = new b2World(b2Vec2(0.0, 9.81));
	addRect(WIDTH / 2, HEIGHT - 50, WIDTH, 30, b2_staticBody);
	int lastFPS = -1;
	bool placed = 0;
	u32 then = device->getTimer()->getTime();
	while (device->run() && execute)
	{
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht + Box2D libraries example! Driver: ";
			str += driver->getName();
			str += " FPS: ";
			str += fps;
			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
		then = now;
		if (receiver.GetMouseState().LeftButtonDown)
		{
			if (!placed)
			{
				addRect(receiver.GetMouseState().Position.X, receiver.GetMouseState().Position.Y, 20, 20);
				placed = !placed;
			}
		}
		else if (placed)placed = !placed;
		drawWorld();
		world->Step(frameDeltaTime, 8, 3);
		device->sleep(5);
	}
	device->drop();
	return 0;
}