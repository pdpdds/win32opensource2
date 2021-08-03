// flatbuffersEx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "flatbuffers/flatbuffers.h"

#include "monster_generated.h"

using namespace MyGame;

int _tmain(int argc, _TCHAR* argv[])
{
	//데이터를 직렬화하는 버퍼 빌더를 선언
	flatbuffers::FlatBufferBuilder builder;

	auto vec = Vec3(1, 2, 3);

	auto name = builder.CreateString("Devil Inside");

	unsigned char inv_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto inventory = builder.CreateVector(inv_data, 10);

	MonsterBuilder mb(builder);
	mb.add_pos(&vec);
	mb.add_hp(80);
	mb.add_name(name);
	mb.add_inventory(inventory);
	auto mloc = mb.Finish();

	builder.Finish(mloc);
	
	auto monster = GetMonster(builder.GetBufferPointer());
	std::cout << "monster name : " << monster->name()->c_str() << std::endl;

	return 0;
}

