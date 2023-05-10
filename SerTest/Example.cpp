#include "Example.h"

Writer Person::serializeCurrent() const {
	return [this](AbstractSerializer& s) {
		auto structSer = s.sStruct();
		structSer->begin();
		structSer->sKeyVal("name", write(_name));
		structSer->sKeyVal("age", write(_age));
		structSer->sKeyVal("sex", write(_sex));
		structSer->finish();
	};
}

Deserialize Person::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<std::string> name;
		DeserializeBuffer<unsigned int> age;
		DeserializeBuffer<Sex> sex;
		auto structDe = de.dStruct();
		structDe->begin();
		structDe->next(scope, "name", deserialize(name.buffer()));
		structDe->next(scope, "age", deserialize(age.buffer()));
		structDe->next(scope, "sex", deserialize(sex.buffer()));
		structDe->end();
		new (buffer.getBuffer()) Person(*name, *age, *sex);
	};
}

void Person::show(std::ostream& o)
{
	o << _name << ", " << _age << ", " << _sex;
}

Writer Student::serializeCurrent() const
{
	return [this](AbstractSerializer& s) {
		auto structSer = s.sStruct();
		structSer->begin();
		structSer->sKeyVal("Person", this->Person::serializeCurrent());
		structSer->sKeyVal("languages", write(_languages));
		structSer->finish();
	};
}

Deserialize Student::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<Person> person;
		DeserializeBuffer<std::vector<std::string>> languages;
		auto structDe = de.dStruct();
		structDe->begin();
		structDe->next(scope, "Person", deserialize(person.buffer()));
		structDe->next(scope, "languages", deserialize(languages.buffer()));
		structDe->end();
		new (buffer.getBuffer()) Student(*person, *languages);
	};
}

void Student::show(std::ostream& o)
{
	o << (Person&)(*this) << ' ';
	for (size_t i = 0; i < _languages.size(); i++)
	{
		o << _languages[i] << ", ";
	}
}


Writer Color::serilaizeCurrent() const
{
	return [this](AbstractSerializer& se) {
		auto structSer = se.sStruct();
		structSer->begin();
		structSer->sKeyVal("red", write(_red));
		structSer->sKeyVal("green", write(_green));
		structSer->sKeyVal("blue", write(_blue));
		structSer->finish();
	};
}

Deserialize Color::read(BufferAlloc& buffer)
{
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<char> red;
		DeserializeBuffer<char> green;
		DeserializeBuffer<char> blue;
		auto structSer = de.dStruct();
		structSer->begin();
		structSer->next(scope, "red", deserialize(red.buffer()));
		structSer->next(scope, "green", deserialize(green.buffer()));
		structSer->next(scope, "blue", deserialize(blue.buffer()));
		structSer->end();
		new (buffer.getBuffer()) Color(*red, *green, *blue);
	};
}
