#include "furniture.h"
#include "SerializeExt.h"
#include "DeserializeExt.h"


Writer Chair::_serialize() const
{
	return serializeCurrent();
}

Writer Chair::serializeCurrent() const
{
	return [this](AbstractSerializer& serializer) {
		auto stSer = serializer.sStruct();
		stSer->begin();
		stSer->sKeyVal("name", write(_name));
		stSer->sKeyVal("frameMaterial", write(_frameMaterial));
		stSer->sKeyVal("outerMaterial", write(_outerMaterial));
		stSer->sKeyVal("reclining", write(_reclining));
		stSer->finish();
	};
}

Deserialize Chair::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<string> name;
		DeserializeBuffer<string> frameMaterial;
		DeserializeBuffer<optional<string>> outerMaterial;
		DeserializeBuffer<bool> reclining;
		auto stSer = de.dStruct();
		stSer->begin();
		stSer->next(scope, "name", deserialize(name.buffer()));
		stSer->next(scope, "frameMaterial", deserialize(frameMaterial.buffer()));
		stSer->next(scope, "outerMaterial", deserialize(outerMaterial.buffer()));
		stSer->next(scope, "reclining", deserialize(reclining.buffer()));
		stSer->end();
		new (buffer.getBuffer()) Chair(*name, *frameMaterial, *outerMaterial, *reclining);
	};
}

Writer Table::_serialize() const
{
	return serializeCurrent();
}

Writer Table::serializeCurrent() const
{
	return [this](AbstractSerializer& serializer) {
		auto stSer = serializer.sStruct();
		stSer->begin();
		stSer->sKeyVal("name", write(_name));
		stSer->sKeyVal("material", write(_material));
		stSer->finish();
	};
}

Deserialize Table::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<string> name;
		DeserializeBuffer<string> material;
		auto stSer = de.dStruct();
		stSer->begin();
		stSer->next(scope, "name", deserialize(name.buffer()));
		stSer->next(scope, "material", deserialize(material.buffer()));
		stSer->end();
		new (buffer.getBuffer()) Table(*name, *material);
	};
}

Writer Bed::_serialize() const
{
	return serializeCurrent();
}

Writer Bed::serializeCurrent() const
{
	return [this](AbstractSerializer& serializer) {
		auto stSer = serializer.sStruct();
		stSer->begin();
		stSer->sKeyVal("name", write(_name));
		stSer->sKeyVal("includesMattress", write(_includesMattress));
		stSer->sKeyVal("peopleCount", write(_peopleCount));
		stSer->finish();
	};
}

Deserialize Bed::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<string> name;
		DeserializeBuffer<bool> includesMattress;
		DeserializeBuffer<uint32_t> peopleCount;
		auto stSer = de.dStruct();
		stSer->begin();
		stSer->next(scope, "name", deserialize(name.buffer()));
		stSer->next(scope, "includesMattress", deserialize(includesMattress.buffer()));
		stSer->next(scope, "peopleCount", deserialize(peopleCount.buffer()));
		stSer->end();
		new (buffer.getBuffer()) Bed(*name, *includesMattress, *peopleCount);
	};
}
