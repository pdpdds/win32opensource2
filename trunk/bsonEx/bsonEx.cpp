// bsonEx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "bson.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace bson;

int _tmain(int argc, _TCHAR* argv[])
{
	bob bsonObjectBuilder;
	bsonObjectBuilder.append("name", "Monster");
	bsonObjectBuilder.append("mana", 10);
	bsonObjectBuilder.append("hp", 62.3);
	bsonObjectBuilder.append("x", 1.0);
	bsonObjectBuilder.append("y", 8.0);
	
	bo bsonObject = bsonObjectBuilder.obj();

	string json = bsonObject.toString();
	cout << "Monster Description : " << json << endl;

	return 0;
}

