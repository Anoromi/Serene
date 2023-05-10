#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>
#include <functional>
#include <optional>

class AbstractSerializer;
class AbstractMapSer;
class AbstractSeqSer;
class AbstractStructSer;

using Writer = std::function<void(AbstractSerializer&)>;

class AbstractSerializer {
protected:
	virtual ~AbstractSerializer() {}
private:
	virtual std::unique_ptr<AbstractMapSer> _sMap(size_t) = 0;
	virtual std::unique_ptr<AbstractSeqSer> _sSeq(size_t) = 0;
	virtual std::unique_ptr<AbstractStructSer> _sStruct() = 0;

	virtual void _sLong(long long) = 0;
	virtual void _sULong(unsigned long long) = 0;
	virtual void _sString(const std::string&) = 0;
	virtual void _sBool(bool) = 0;
	virtual void _sChar(char) = 0;
public:

	inline std::unique_ptr<AbstractMapSer> sMap(size_t s) { return _sMap(s); }
	inline std::unique_ptr<AbstractSeqSer> sSeq(size_t s) { return _sSeq(s); };
	inline std::unique_ptr<AbstractStructSer> sStruct() { return _sStruct(); };

	inline void sLong(long long v) { return _sLong(v); };
	inline void sULong(unsigned long long v) { return _sULong(v); };
	inline void sString(std::string v) { return _sString(v); };
	inline void sBool(bool v) { return _sBool(v); };
	inline void sChar(char v) { return _sChar(v); };

};



template<typename T>
concept Serializable = requires(T t) {
	{write(t)};
};

class AbstractMapSer {
private:
	virtual void _sKeyVal(const std::string&, const Writer&) = 0;
	virtual void _begin() = 0;
	virtual void _finish() = 0;

public:
	virtual ~AbstractMapSer() {}
	inline void sKeyVal(const std::string& k, const Writer& v) { return _sKeyVal(k, v); };
	void begin() { return _begin(); };
	void finish() { return _finish(); };

};


class AbstractSeqSer {
private:

	virtual void _sNext(const Writer& v) = 0;
	virtual void _begin() = 0;
	virtual void _finish() = 0;
public:
	virtual ~AbstractSeqSer() {}

	inline void sNext(const Writer& v) { return _sNext(v); }
	inline void begin() { return _begin(); };
	inline void finish() { return _finish(); };
};

class AbstractStructSer {
private:
	virtual void _sKeyVal(const std::string&, const Writer&) = 0;
	virtual void _begin() = 0;
	virtual void _finish() = 0;

public:
	virtual ~AbstractStructSer() {}

	inline void sKeyVal(const std::string& k, const Writer& v) { return _sKeyVal(k, v); };
	inline void begin() { return _begin(); };
	inline void finish() { return _finish(); };
};


inline Writer write(std::same_as<long long> auto v) {
	return [v](AbstractSerializer& s) {s.sLong(v); };
}

inline Writer write(std::same_as<unsigned long long> auto v) {
	return [v](AbstractSerializer& s) {s.sULong(v); };
}

inline Writer write(std::same_as<int> auto v) {
	return write((long long)v);
}

inline Writer write(std::same_as<unsigned int> auto v) {
	return write((unsigned long long) v);
}



inline Writer write(const std::same_as<std::string> auto& v) {
	return [&v](AbstractSerializer& s) {s.sString(v); };
}

inline Writer write(const std::same_as<const char* const> auto& v) {
	return [&v](AbstractSerializer& s) {s.sString(v); };
}


inline Writer write(std::same_as<bool> auto v) {

	return [v](AbstractSerializer& s) {s.sBool(v); };
}

inline Writer write(std::same_as<char> auto v) {
	return [v](AbstractSerializer& s) {s.sChar(v); };
}


//template<typename T>
//concept HasClassName = requires(T t) {
//	{T::typeName} -> std::convertible_to<const char(&)[]>;
//};

class SerializableObject {
private:
	virtual Writer _serialize() const = 0;
	virtual const char* _getObjectName() const = 0;
public:
	inline Writer serialize() const { return _serialize(); }
	inline const char* getObjectName() const { return _getObjectName(); }
};

template<typename T>
requires std::derived_from<T, SerializableObject>
inline Writer write(const T* v) {
	return [v](AbstractSerializer& s) {
		auto structSer = s.sStruct();
		structSer->begin();
		structSer->sKeyVal("type", write<std::string>(v->getObjectName()));
		structSer->sKeyVal("value", write<T>(*v));
		structSer->finish();
	};
}


template<typename T>
requires (!std::derived_from<T, SerializableObject>)
inline Writer write(const T* v) {
	return [v](AbstractSerializer& s) {
		auto structSer = s.sStruct();
		structSer->begin();
		structSer->sKeyVal("value", write<T>(*v));
		structSer->finish();
	};
}





template<typename T>
concept SerializableIterator = requires(T t) {
	T::reference;

	Serializable<typename T::reference>;
	{ ++t } -> std::same_as<T>;
	{ *t } -> std::same_as<typename T::reference>;
};

template<typename T>
concept SerializableSeq = requires(T t) {
	SerializableIterator<decltype(t.begin())>;
	SerializableIterator<decltype(t.end())>;
	{t.size()} -> std::same_as<size_t>;
};

template<typename T>
concept SerializableMap = requires(T t) {
	SerializableIterator<decltype(t.begin())>;
	SerializableIterator<decltype(t.end())>;
	{t.size()} -> std::same_as<size_t>;
	typename T::key_type;
	typename T::value_type;
	{*t.begin()} -> std::same_as < std::pair<typename T::key_type, typename T::value_type>>;

};

template<typename T>
	requires  (!std::same_as<T, std::string>) && SerializableSeq<T> && (!std::derived_from<T, SerializableObject>)
inline Writer write(const T& v) {
	return [&v](AbstractSerializer& s) {
		auto seqSer = s.sSeq(v.size());
		seqSer->begin();
		for (auto iter = v.begin(); iter != v.end(); ++iter) {
			auto& current = *iter;
			seqSer->sNext(write(current));
		}
		seqSer->finish();
	};
}

template<typename T>
	requires  (!std::same_as<T, std::string>) && SerializableMap<T> && (!std::derived_from<T, SerializableObject>)
inline Writer write(const T& v) {
	return [&v](AbstractSerializer& s) {
		auto seqSer = s.sSeq(v.size());
		seqSer->begin();
		for (auto iter = v.begin(); iter != v.end(); ++iter) {
			auto& current = *iter;
			seqSer->sNext(write(current));
		}
		seqSer->finish();
	};
}

template<typename T>
	requires std::derived_from<T, SerializableObject>
inline Writer write(const T& v) {
	return v.serialize();
}





struct ExceededSize {};


