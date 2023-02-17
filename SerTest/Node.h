#pragma once
#include "Serializer.h"

template<typename T>
class Node {
private:
	T _value;
	std::unique_ptr<Node<T>> _next;
public:
	Node(const T& value) : _value(value), _next() {}
	Node& connect(const T& nextValue);

	template<Serializer S>
	void save(S& ser);
};

template<typename T>
inline Node<T>& Node<T>::connect(const T& nextValue)
{
	_next.reset(new Node(nextValue));
	return *_next;
}

template<typename T>
template<Serializer S>
inline void Node<T>::save(S& ser)
{
	auto seqSer = ser.sSeq();
	auto* current = this;
	
}
