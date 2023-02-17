#pragma once
#include "Serializer.h"
#include <iostream>
#include <fstream>

class BinarySer;
class BinaryMapSer;
class BinarySeqSer;
class BinaryStructSer;


using Stream = std::ofstream;
class BinarySer final : public AbstractSerializer<BinarySeqSer, BinaryMapSer, BinaryStructSer> {
private:
	Stream& _out;
public:
	BinarySer(Stream& out) : _out(out) {};
	BinaryMapSer sMap(size_t);
	BinarySeqSer sSeq(size_t);
	BinaryStructSer sStruct();
	void sLong(long long);
	void sULong(unsigned long long);
	void sString(const std::string&);
	void sBool(bool);
	void sChar(char);
};

class BinaryMapSer final : public AbstractMapSer<BinarySer> {
private:
	size_t _size;
	BinarySer& _ser;
	Stream& _out;
public:
	BinaryMapSer(size_t size, BinarySer& ser, Stream& out);

	template<typename T>
		requires Serializable<BinarySer, T>
	void sKeyVal(const std::string&, T);

	template<typename T>
		requires Serializable<BinarySer, T>
	void sKeyVal(unsigned long long, T);

	void begin() {};
	void finish();

};

class BinarySeqSer final : public AbstractSeqSer<BinarySer> {
private:
	size_t _size;
	Stream& _out;
	BinarySer& _ser;
public:
	BinarySeqSer(size_t size, BinarySer& ser, Stream& out);
	~BinarySeqSer();

	template<typename T>
		requires Serializable<BinarySer, T>
	inline void sNext(T t);

	void begin() {};
	void finish();
};

class BinaryStructSer final : public AbstractStructSer<BinarySer> {
private:
	Stream& _out;
	BinarySer& _ser;
public:
	BinaryStructSer(BinarySer& ser, Stream& out);

	template<typename T>
		requires Serializable<BinarySer, T>
	void sKeyVal(const std::string&, T);

	template<typename T>
		requires Serializable<BinarySer, T>
	void sKeyVal(unsigned long long, T);

	void begin() {};
	void finish();
};

template<typename T>
	requires Serializable<BinarySer, T>
inline void BinarySeqSer::sNext(T t)
{
	if (_size-- == 0)
		throw ExceededSize();
	write(_ser, t);
}

template<typename T>
	requires Serializable<BinarySer, T>
inline void BinaryMapSer::sKeyVal(const std::string& k, T t)
{
	if (_size-- == 0)
		throw ExceededSize();
	write(_ser, k);
	write(_ser, t);
}

template<typename T>
	requires Serializable<BinarySer, T>
inline void BinaryMapSer::sKeyVal(unsigned long long k, T t)
{
	if (_size-- == 0)
		throw ExceededSize();
	write<BinarySer, unsigned long long>(_ser, k);
	write<BinarySer, T>(_ser, t);
}

template<typename T>
	requires Serializable<BinarySer, T>
inline void BinaryStructSer::sKeyVal(const std::string& k, T t)
{
	write<BinarySer, std::string>(_ser, k);
	write<BinarySer, T>(_ser, t);
}

template<typename T>
	requires Serializable<BinarySer, T>
inline void BinaryStructSer::sKeyVal(unsigned long long, T t)
{
	write(_ser, t);
}
