#pragma once
#include "Deserializer.h"
#include <fstream>

class JSONDe;
class JSONMapDe;
class JSONSeqDE;

using Stream = std::ifstream;

//class JSONMapValue : AbstractMapValue {
//protected:
//	template<DeVisitor Visitor>
//	Visitor::Return parseValue(Visitor&);
//
//};

class JSONMapDe : AbstractMapDe<JSONDe> {
public:

};


class JSONDe : AbstractDeserializer {
private:
	Stream _in;
public:
	template<DeVisitor Visitor>
	Visitor::Return nextValue(Visitor&);
};


inline char skipWhitespace(Stream s) {
	char n;
	s >> n;
	while (n == ' ' || n == '\r' || n == '\n') {
		s >> n;
	}
}


inline char matches(char f, char s) {
	if (f != s) {
		throw "Mismatched symbols";
	}
}

template<DeVisitor Visitor>
inline Visitor::Return JSONDe::nextValue(Visitor& v)
{
	char next = skipWhitespace(_in);
	switch (next)
	{
	case '[':

		break;
	case '{':
		break;
	case '"':
		break;
	case 't':
	case 'f':
		break;
		//case 
	default:
		break;
	}
	//if (next == '[') {	
	//}
	//else if (next == '{') {

	//}
	//else if (next == '"') {

	//}
	//else if (next == 't') {

	//}
	return Visitor::Return();
}
