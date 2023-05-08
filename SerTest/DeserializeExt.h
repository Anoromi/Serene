#pragma once
#include "Deserializer.h"
#include <list>

//template<Deserializable D>
//class SeqIterator {
//private:
//	AbstractSeqDe& _item;
//
//public:
//};




template<typename T>
concept PushDeserialize = requires(T t, const typename T::value_type & g) {
	Deserializable<typename T::value_type>;
	{t.push_back(g) };
};

template<PushDeserialize T>
	requires (!std::same_as<std::string, T>)
Deserialize read(BufferAlloc& buffer) {
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		//read<T>(buffer);
		auto seq = new (buffer.getBuffer()) T();
		auto seqDe = de.dSeq();
		seqDe->begin();
		while (seqDe->hasMore()) {
			DeserializeBuffer<typename T::value_type> buffer;
			seqDe->nextItem(scope, deserialize(buffer.buffer()));
			seq->push_back(*buffer);

		}
		seqDe->end();
	};
}



inline void hello() {

	std::list<int> v;


	DeserializeBuffer<std::list<std::string>> hehe;
	deserialize(hehe.buffer());
	//std::list<int>::value_type
		//v.end();
}
