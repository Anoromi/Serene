#pragma once
#include <memory>
#include "furniture.h"


class Configuration : public SerializableObject {
	RegisterSerializeClass(Configuration);
private:
	int _port;
	bool _production;
	virtual Writer _serialize() const { return serializeCurrent(); }
public:
	Configuration(int port, bool production) : _port(port), _production(production) {}

	Writer serializeCurrent() const;

	static Deserialize read(BufferAlloc&);
	int port() const { return _port; }
	bool inProduction() const { return _production; }


};

Configuration loadConfig();

void updateCache();

optional<std::shared_ptr<Furniture>> getFurniture(const std::string& name);

void insertFurniture(const Furniture& f);

void loadTestData();


