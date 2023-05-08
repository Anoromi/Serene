#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>
#include <functional>
#include "Serializer.h"
#include "BufferAlloc.h"
#include "StaticInit.h"

class AbstractDeserializer;

class DeserialzationScope {
private:
	std::vector<std::map<std::string, void*>> scope;
public:
	inline void push() { scope.push_back({}); };
	inline void pop() { scope.pop_back(); };
	void* read(const std::string& k) const;
	inline void write(const std::string& k, void* v) { scope.back().insert(std::pair(k, v)); };

};

using Deserialize = std::function<void(AbstractDeserializer&, DeserialzationScope&)>;


class AbstractMapDe {
private:
	virtual bool _hasMore() = 0;
	virtual std::string _nextKey() = 0;
	virtual void _nextValue(DeserialzationScope&, const Deserialize&) = 0;
	virtual void _begin() = 0;
	virtual void _end() = 0;
public:
	virtual ~AbstractMapDe() {};
	bool hasMore() { return _hasMore(); }
	std::string nextKey() { return _nextKey(); }
	void nextValue(DeserialzationScope& scope, const Deserialize& de) { _nextValue(scope, de); }
	void begin() { _begin(); }
	void end() { _end(); }
};

class AbstractSeqDe {
private:
	virtual bool _hasMore() = 0;
	virtual void _nextItem(DeserialzationScope&, const Deserialize&) = 0;
	virtual void _begin() = 0;
	virtual void _end() = 0;
public:
	virtual ~AbstractSeqDe() {};
	bool hasMore() { return _hasMore(); }
	void nextItem(DeserialzationScope& scope, const Deserialize& de) { _nextItem(scope, de); }
	void begin() { _begin(); }
	void end() { _end(); }
};

class AbstractStructDe {
private:
	virtual void _next(DeserialzationScope& scope, const std::string&, const Deserialize&) = 0;
	virtual void _begin() = 0;
	virtual void _end() = 0;
public:
	void next(DeserialzationScope& scope, const std::string& key, const Deserialize& de) { _next(scope, key, de); }
	void begin() { _begin(); }
	void end() { _end(); }
};


class AbstractDeserializer {
private:
	virtual long long _dLong() = 0;
	virtual unsigned long long _dULong() = 0;
	virtual std::string _dString() = 0;
	virtual bool _dBool() = 0;
	virtual char _dChar() = 0;
	virtual std::unique_ptr<AbstractMapDe> _dMap() = 0;
	virtual std::unique_ptr<AbstractSeqDe> _dSeq() = 0;
	virtual std::unique_ptr<AbstractStructDe> _dStruct() = 0;

public:
	virtual ~AbstractDeserializer() {};
	long long dLong() { return _dLong(); }
	unsigned long long dULong() { return _dULong(); }
	std::string dString() { return _dString(); }
	bool dBool() { return _dBool(); }
	char dChar() { return _dChar(); }
	std::unique_ptr<AbstractMapDe> dMap() { return _dMap(); }
	std::unique_ptr<AbstractSeqDe> dSeq() { return _dSeq(); }
	std::unique_ptr<AbstractStructDe> dStruct() { return _dStruct(); }
};



class DeserializationException : public std::exception {
private:
	std::string message;

public:
	explicit DeserializationException(const std::string& msg) : message(msg) {}

	const char* what() const noexcept override {
		return message.c_str();
	}

};

template<typename T>
concept Deserializable = requires(T t, BufferAlloc & buffer) {
	{T::read(buffer)};
};

using PointerDeserialize = Deserialize(*)(HeapAlloc&);

class DeserializationDescription {
private:
	PointerDeserialize _deserialize;
	unsigned long long _objectSize;
public:
	DeserializationDescription(unsigned long long objectSize, PointerDeserialize& deserialize) : _objectSize(objectSize), _deserialize(deserialize) {}
	const PointerDeserialize& getDeserializer() const { return _deserialize; }
	const size_t getObjectSize() const { return _objectSize; }
};

class TypeAccumulator {
private:
	std::map<std::string, DeserializationDescription> _map;
	static TypeAccumulator* instance;
public:
	static TypeAccumulator& getInstance() {
		static TypeAccumulator instance;
		return instance;
	}

	inline void add(const std::string& s, const DeserializationDescription& d) { _map.insert(std::make_pair(s, d)); };
	inline const DeserializationDescription& find(const std::string& s) const { return _map.at(s); };
};

template<typename T>
Deserialize read(BufferAlloc&);

template<typename T>
	requires std::same_as<T, long long>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) long long(de.dLong());
	};
}

template<typename T>
	requires std::same_as<T, int>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) int(de.dLong());
	};
}

template<typename T>
	requires std::same_as<T, unsigned long long>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) unsigned long long(de.dLong());
	};
}

template<typename T>
	requires std::same_as<T, unsigned int>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) unsigned int(de.dULong());
	};
}



template<typename T>
	requires std::same_as<T, std::string>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) std::string(de.dString());
	};
}

template<typename T>
	requires std::same_as<T, char>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) char(de.dChar());
	};
}

template<typename T>
	requires std::same_as<T, bool>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope&) {
		new (v.getBuffer()) bool(de.dBool());
	};
}

template<typename T>
	requires Deserializable<T>
Deserialize read(BufferAlloc& v) {
	return T::read(v);
}

template<typename T>
Deserialize deserialize(StackAlloc<T>& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope& scope) {
		read<T>(v)(de, scope);
		v.setWasInitialized(true);
	};
}

template<typename T>
Deserialize deserialize(HeapAlloc& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope& scope) {
		read<T>(v)(de, scope);
	};
}

template<typename T>
	requires std::derived_from<T, SerializableObject>
Deserialize deserialize(T*& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope& scope) {

		StackAlloc<std::string> type;
		auto str = de.dStruct();
		str->next(scope, "type", deserialize(type));

		std::cout << *type;

		const DeserializationDescription& description = TypeAccumulator::getInstance().find(*type);
		HeapAlloc obj(description.getObjectSize());
		str->next(scope, "value", description.getDeserializer()(obj));
		v = reinterpret_cast<T*>(obj.getBuffer());
	};
}

template<typename T>
concept HasRead = requires(T t, HeapAlloc & buffer) {
	{read<T>(buffer) };
};

template<HasRead T>
	requires (!std::derived_from<T, SerializableObject>)
Deserialize deserialize(T*& v) {
	return [&v](AbstractDeserializer& de, DeserialzationScope& scope) {
		HeapAlloc obj(sizeof(T));
		read<T>(obj);
		v = reinterpret_cast<T*>(obj.getBuffer());
	};
}

template<typename T>
Deserialize heapDeserialize(HeapAlloc& buffer) {
	return read<T>(buffer);
}
template<char const* name, typename T>
void registerClassDeserialize() {

	auto de = &heapDeserialize<T>;
	TypeAccumulator::getInstance().add(name, DeserializationDescription(sizeof(T), de));
}

#define RegisterSerializeClass(type) \
static constexpr char typeName[] = #type; \
static inline StaticInit<registerClassDeserialize<typeName, type>> init;





template<typename T>
class DeserializeBuffer;

template<typename T>
	requires std::is_pointer<T>::value&& std::derived_from<std::remove_pointer_t<T>, SerializableObject>
class DeserializeBuffer<T> {
private:
	T _value = nullptr;
public:
	inline T& buffer() { return _value; }
	T& operator*() {
		return _value;
	}
};

template<typename T>
	requires (!std::is_pointer<T>::value || !std::derived_from<std::remove_pointer_t<T>, SerializableObject>)
class DeserializeBuffer<T> {
private:
	StackAlloc<T> _buffer;
public:
	inline StackAlloc<T>& buffer() { return _buffer; }
	T&& operator*() {
		return  *_buffer;
	}
};

