#pragma once
#include <string>
#include "Serializer.h"
#include <optional>
#include "Deserializer.h"
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

using std::string;
using std::optional;


class Furniture : public SerializableObject {
protected:
	string _name;
	//double _width;
	//double _height;
	//double _length;
public:
	Furniture(string name) : _name(name) {}
	virtual ~Furniture() {}

	inline const string& name() const { return _name; }

	//template<class Archive>
	//void serialize(Archive& ar)
	//{
	//	ar(_name);
	//}
};

class Chair : public Furniture {
	RegisterSerializeClass(Chair);
private:
	string _frameMaterial;
	optional<string> _outerMaterial;
	bool _reclining;
	virtual Writer _serialize() const;

public:
	Chair(string name, string frameMaterial, optional<string> outerMaterial, bool reclining) :
		Furniture(name), _frameMaterial(frameMaterial), _outerMaterial(outerMaterial), _reclining(reclining) {}

	Writer serializeCurrent() const;
	static Deserialize read(BufferAlloc&);

	//template<class Archive>
	//void serialize(Archive& ar)
	//{
	//	ar(cereal::base_class<Furniture>(this), _frameMaterial, _outerMaterial, _reclining);
	//}

};

class Table : public Furniture {
	RegisterSerializeClass(Table);
private:
	string _material;
	virtual Writer _serialize() const;
public:
	Table(string name, string material) : Furniture(name), _material(material) {}

	Writer serializeCurrent() const;
	static Deserialize read(BufferAlloc&);

	//template<class Archive>
	//void serialize(Archive& ar)
	//{
	//	ar(cereal::base_class<Furniture>(this), _material);
	//}
};

class Bed : public Furniture {
	RegisterSerializeClass(Bed);
private:
	bool _includesMattress;
	uint32_t _peopleCount;
	virtual Writer _serialize() const;
public:
	Bed(string name, bool includesMattresse, uint32_t peopleCount) :
		Furniture(name), _includesMattress(includesMattresse), _peopleCount(peopleCount) {}

	Writer serializeCurrent() const;
	static Deserialize read(BufferAlloc&);

	//template<class Archive>
	//void serialize(Archive& ar)
	//{
	//	ar(cereal::base_class<Furniture>(this), _includesMattress, _peopleCount);
	//}
};

//CEREAL_REGISTER_TYPE(Bed);
//CEREAL_REGISTER_TYPE(Chair);
//CEREAL_REGISTER_TYPE(Table);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Furniture, Bed);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Furniture, Chair);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Furniture, Table);

