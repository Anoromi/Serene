#pragma once
#include <string>
#include "Serializer.h"


struct Person {
	std::string _name;
	unsigned int _age;
	char _gender;
	std::vector<std::string> _languages;
};


template<Serializer S, std::same_as<Person> T>
void write(S& ser, const T& p) {
	auto structSer = ser.sStruct();
	structSer.begin();
	structSer.sKeyVal("name", p._name);
	structSer.sKeyVal("age", (unsigned long long) p._age);
	structSer.sKeyVal("gender", p._gender);
	structSer.sKeyVal("languages", p._languages);
	structSer.finish();
}

