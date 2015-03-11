// cerealEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>

struct MyRecord
{
	uint8_t x, y;
	float z;

	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(x, y, z);
	}
};

struct SomeData
{
	int32_t id;
	std::unordered_map<uint32_t, MyRecord> data;

	template <class Archive>
	void save(Archive & ar) const
	{
		ar(data);
	}

	template <class Archive>
	void load(Archive & ar)
	{
		static int32_t idGen = 0;
		id = idGen++;
		ar(data);
	}
};

int main()
{
	std::ofstream os("out.cereal", std::ios::binary);
	cereal::BinaryOutputArchive archive(os);

	SomeData myData;
	MyRecord record;
	record.x = 5;
	record.y = 10;
	record.z = 2.5f;
	myData.data.insert(std::make_pair(1, record));
	archive(myData);
	os.close();


	std::ifstream is("out.cereal", std::ios::binary);
	cereal::BinaryInputArchive inputArchive(is);

	SomeData outData;
	inputArchive(outData);
	is.close();

	return 0;
}