#pragma once
#include "Deserializer.h"
#include <list>

template<typename T>
concept PushDeserialize = requires(T t, const typename T::value_type & g) {
	Deserializable<typename T::value_type>;
	{t.push_back(g) };
};

template<PushDeserialize T>
	requires (!std::same_as<std::string, T>) && (!std::derived_from<T, SerializableObject>)
Deserialize read(BufferAlloc& buffer) {
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
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


template<typename V>
	requires std::same_as<std::optional<typename V::value_type>, V>
Deserialize read(BufferAlloc& buffer) {
	return [&buffer](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<bool> hasValue;
		auto st = de.dStruct();
		st->next(scope, "hasValue", deserialize(hasValue.buffer()));
		if (*hasValue) {
			DeserializeBuffer<typename V::value_type> value;
			st->next(scope, "value", deserialize(value.buffer()));
			new (buffer.getBuffer()) V(*value);
		}
		else {
			new (buffer.getBuffer()) V();
		}
	};
}


template<typename T>
	requires std::same_as<T, std::unique_ptr<typename T::element_type>>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<typename T::element_type*> value;
		deserialize(value.buffer())(de, scope);
		new (v.getBuffer()) T(*value);
	};
}

template<typename T>
	requires std::same_as<T, std::shared_ptr<typename T::element_type>>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope& scope) {
		DeserializeBuffer<typename T::element_type*> value;
		deserialize(value.buffer())(de, scope);
		new (v.getBuffer()) T(*value);
	};
}
