#pragma once
#include <string>

class AbstractDeserializer;
class AbstractSeqDe;
template<typename MapValue>
class AbstractMapDe;

template<typename T>
class AbstractDeVisitor {
protected:
	T visitULong(unsigned long long);
	T visitLong(long long);
	T visitString(std::string);
	T visitSeq(AbstractDeserializer&);
	T visitStruct(AbstractDeserializer&);
	T visitMap(AbstractDeserializer&);
	T visitDefault(AbstractDeserializer&);
public:
	using Return = T;
};

class AbstractSeqDe {
protected:
	bool hasMore();
	template<typename T, DeVisitor Visitor>
	Visitor::Return nextItem(Visitor&);
};

//class AbstractMapValue {
//protected:
//	template<DeVisitor Visitor>
//	Visitor::Return parseValue(Visitor&);
//};

template<typename MapValue>
class AbstractMapDe {
protected:
	bool hasMore();
	template<typename T, DeVisitor Visitor>
	std::pair<typename Visitor::Return, MapValue> nextKey(Visitor&);
};


template<typename T>
concept DeVisitor = requires(T t) {
	T::Return;
	std::derived_from<T, AbstractDeVisitor<typename T::Return>>;
};

class AbstractDeAccess {
	unsigned long long dUllong();
	long long dLLong();
	std::string dString();
	char dChar();


};

class AbstractDeserializer {
protected:
	template<DeVisitor Visitor>
	Visitor::Return nextValue(Visitor&);
};

