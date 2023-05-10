#include "access.h"
#include <fstream>
#include <format>
#include "BinaryDe.h"
#include "BinarySer.h"
#include "DeserializeExt.h"
#include <sstream>
#include <boost/json.hpp>
#include "JSONDe.h"




std::mutex cacheMutex;
std::map<std::string, std::string> cashedFurniture;

Configuration loadConfig()
{
	DeserializeBuffer<Configuration> configurationBuffer;
	std::ifstream configReader("./furnitureOther/config.json");
	boost::json::value v(boost::json::parse(configReader));
	JSONDe de(v);
	DeserialzationScope scope;
	deserialize(configurationBuffer.buffer())(de, scope);

	return *configurationBuffer;
}

void updateCache()
{
	cacheMutex.lock();
	cashedFurniture.clear();
	cacheMutex.unlock();
}

std::shared_ptr<Furniture> loadFromCache(const std::string& name) {
	DeserializeBuffer<Furniture*> furniture;
	std::istringstream stream(cashedFurniture.at(name));
	BinaryDe de(stream);
	DeserialzationScope scope;
	deserialize(furniture.buffer())(de, scope);
	return std::shared_ptr<Furniture>(*furniture);
}

void addToCache(const Furniture& furniture) {
	cacheMutex.lock();
	{
		std::ostringstream stream;
		BinarySer ser(stream);
		write(furniture)(ser);
		cashedFurniture.emplace(furniture.name(), stream.str());
	}
	cacheMutex.unlock();

}

optional<std::shared_ptr<Furniture>> getFurniture(const std::string& name)
{
	cacheMutex.lock();

	if (cashedFurniture.contains(name)) {
		auto v = loadFromCache(name);
		cacheMutex.unlock();
		return v;
	}
	else {
		cacheMutex.unlock();
	}


	string path = std::format("furnitures/{}", name);
	std::ifstream file(path);
	if (!file.is_open())
		return optional<std::shared_ptr<Furniture>>();
	BinaryDe de(file);
	DeserialzationScope scope;
	DeserializeBuffer<Furniture*> furniture;
	deserialize(furniture.buffer())(de, scope);

	std::shared_ptr<Furniture> result(*furniture);

	addToCache(*result);

	return result;
}

void insertFurniture(const Furniture& f)
{
	cacheMutex.lock();
	cashedFurniture.erase(f.name());
	cacheMutex.unlock();

	string path = std::format("furnitures/{}", f.name());
	std::ofstream file(path);
	file.clear();
	BinarySer ser(file);
	write(&f)(ser);
}

void loadTestData()
{

	DeserializeBuffer<std::vector<std::unique_ptr<Furniture>>> furnitures;
	std::ifstream stream("./furnitureOther/mockData.json");
	boost::json::value v(boost::json::parse(stream));
	JSONDe de(v);
	DeserialzationScope scope;
	deserialize(furnitures.buffer())(de, scope);

	auto items = *furnitures;
	for (auto& v : items) {
		insertFurniture(*v);
	}
}



Writer Configuration::serializeCurrent() const
{
	return [this](AbstractSerializer& serializer) {
		auto stSer = serializer.sMap(2);
		stSer->begin();
		stSer->sKeyVal("port", write(_port));
		stSer->sKeyVal("production", write(_production));
		stSer->finish();
	};
}

Deserialize Configuration::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<int> port;
		bool hasPort = false;
		DeserializeBuffer<bool> production;
		bool hasProduction = false;
		auto stSer = de.dMap();
		stSer->begin();
		while (stSer->hasMore())
		{
			auto next = stSer->nextKey();
			if (next == "port") {
				stSer->nextValue(scope, deserialize(port.buffer()));
				hasPort = true;
			}
			else if (next == "production") {
				stSer->nextValue(scope, deserialize(production.buffer()));
				hasProduction = true;
			}
		}
		int rPort = hasPort ? *port : 8080;
		bool rProduction = hasProduction ? *production : false;
		stSer->end();
		new (buffer.getBuffer()) Configuration(rPort, rProduction);
	};
}
