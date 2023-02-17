#pragma once
#include "Serializer.h"
#include <fstream>

class JSONSer;
class JSONMapSer;
class JSONSeqSer;
class JSONStructSer;

using Stream = std::ofstream;
class JSONSer
	: public AbstractSerializer<JSONMapSer, JSONSeqSer, JSONStructSer> {
private:
	Stream& _out;
public:
	JSONSer(Stream& out) : _out(out) {};
	JSONMapSer sMap(size_t);
	JSONSeqSer sSeq(size_t);
	JSONStructSer sStruct();
	void sLong(long long);
	void sULong(unsigned long long);
	void sString(const std::string&);
	void sBool(bool);
	void sChar(char);
};

class JSONMapSer : AbstractMapSer<JSONSer> {
private:
	Stream& _out;
	JSONSer& _ser;
	bool addComma = false;
public:
	JSONMapSer(Stream& out, JSONSer& ser) : _out(out), _ser(ser) { }
	~JSONMapSer() { }


	template<typename T>
		requires Serializable<JSONSer, T>
	void sKeyVal(const std::string&, T);

	template<typename T>
		requires Serializable<JSONSer, T>
	void sKeyVal(unsigned long long, T);

	void start();
	void finish();

};

class JSONSeqSer : AbstractSeqSer<JSONSer> {
private:
	Stream& _out;
	JSONSer& _ser;
	bool addComma = false;
public:
	JSONSeqSer(Stream& out, JSONSer& ser) : _out(out), _ser(ser) { }
	~JSONSeqSer() { }

	template<typename T>
		requires Serializable<JSONSer, T>
	void sNext(T);

	void begin();
	void finish();
};

class JSONStructSer : AbstractStructSer<JSONSer> {
private:
	Stream& _out;
	JSONSer& _ser;
	bool addComma = false;
public:
	JSONStructSer(Stream& out, JSONSer& ser) : _out(out), _ser(ser) { }
	~JSONStructSer() { }

	template<typename T>
		requires Serializable<JSONSer, T>
	void sKeyVal(const std::string&, T);

	template<typename T>
		requires Serializable<JSONSer, T>
	void sKeyVal(unsigned long long, T);

	void begin();
	void finish();
};

template<typename T>
	requires Serializable<JSONSer, T>
inline void JSONMapSer::sKeyVal(const std::string& k, T v)
{
	if (addComma)
		_out << ',';
	_ser.sString(k);
	_out << ':';
	write(_ser, v);
	addComma = true;
}

template<typename T>
	requires Serializable<JSONSer, T>
inline void JSONMapSer::sKeyVal(unsigned long long k, T v)
{
	if (addComma)
		_out << ',';
	_out << '"';
	_ser.sULong(k);
	_out << '"' << ':';
	write(_ser, v);
	addComma = true;
}

template<typename T>
	requires Serializable<JSONSer, T>
inline void JSONStructSer::sKeyVal(const std::string& k, T v)
{
	if (addComma)
		_out << ',';
	_ser.sString(k);
	_out << ':';
	write(_ser, v);
	addComma = true;
}

template<typename T>
	requires Serializable<JSONSer, T>
inline void JSONStructSer::sKeyVal(unsigned long long k, T v)
{
	if (addComma)
		_out << ',';
	_out << '"';
	_ser.sULong(k);
	_out << '"' << ':';
	write(_ser, v);
	addComma = true;
}

template<typename T>
	requires Serializable<JSONSer, T>
inline void JSONSeqSer::sNext(T v)
{
	if (addComma)
		_out << ',';
	write(_ser, v);
	addComma = true;
}
