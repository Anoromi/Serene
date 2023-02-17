#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>


template<typename MapSer, typename SeqSer, typename StructSer>
class AbstractSerializer {
protected:
	~AbstractSerializer() {}
protected:
	MapSer sMap(size_t);
	SeqSer sSeq(size_t);
	StructSer sStruct();

	void sLong(long long);
	void sULong(unsigned long long);
	//void sWString(std::wstring);
	void sString(std::string);
	void sBool(bool);
	void sChar(char);


	using SMap = MapSer;
	using SSeq = SeqSer;
	using SStruct = StructSer;
};

template<typename T>
concept Serializer = requires(T t) {
	T::SMap;
	T::SSeq;
	T::SStruct;
	std::derived_from < T, AbstractSerializer<typename T::SMap, typename T::SSeq, typename T::SStruct>>;
};


template<typename S, typename T>
concept Serializable = requires(S s, T t) {
	Serializer<S>;
	{write(s, t)};
};

template<typename S>
class AbstractMapSer {
protected:
	~AbstractMapSer() {}
protected:

	template<typename T>
		requires Serializable<S, T>
	void sKeyVal(const std::string&, T);
	

	template<typename T>
		requires Serializable<S, T>
	void sKeyVal(unsigned long long, T);

	void begin();
	void finish();
};


template<typename S>
class AbstractSeqSer {
protected:
	~AbstractSeqSer() {}
protected:
	template<typename T>
		requires Serializable<S, T>
	void sNext(T);

	void begin();
	void finish();
};

template<typename S>
class AbstractStructSer {
protected:
	~AbstractStructSer() {}
protected:
	template<typename T>
		requires Serializable<S, T>
	void sKeyVal(const std::string&, T);

	template<typename T>
		requires Serializable<S, T>
	void sKeyVal(unsigned long long, T);

	void begin();
	void finish();

};


template<typename S, typename T>
	requires Serializer<S>&& std::same_as<T, long long>
void write(S& s, T v) {
	s.sLong(v);
}

template<typename S, typename T>
	requires Serializer<S>&& std::same_as<T, unsigned long long>
void write(S& s, T v) {
	s.sULong(v);
}


template<typename S, typename T>
	requires Serializer<S>&& std::same_as<T, std::string>
void write(S& s, const T& v) {
	s.sString(v);
}

template<typename S, typename T>
	requires Serializer<S>&& std::same_as<T, bool>
void write(S& s, T v) {
	s.sBool(v);
}

template<typename S, typename T>
	requires Serializer<S>&& std::same_as<T, char>
void write(S& s, T v) {
	s.sChar(v);
}

template<typename S, typename T>
concept SerializableIterator = requires(S s, T t) {
	T::reference;
	Serializable<S, typename T::reference>;
	{ ++t } -> std::same_as<T>;
	{ *t } -> std::same_as<typename T::reference>;
};

template<typename S, typename T>
concept SerializableIterable = requires(S s, T t) {
	SerializableIterator<S, decltype(t.begin())>;
	SerializableIterator<S, decltype(t.end())>;
	{t.size()} -> std::same_as<size_t>;
};



template<typename S, typename T>
	requires Serializer<S> && (!same_as<T, std::string>) && SerializableIterable<S, T>
void write(S& s, const T& v) {

	auto seqSer = s.sSeq(v.size());
	seqSer.begin();
	for (auto iter = v.begin(); iter != v.end(); ++iter)
		seqSer.sNext(*iter);
	seqSer.finish();
}

struct ExceededSize {};
