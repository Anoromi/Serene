#pragma once
#include "Serializer.h"
#include <iostream>
#include <fstream>

class BinarySer;
class BinaryMapSer;
class BinarySeqSer;
class BinaryStructSer;


using Stream = std::ofstream;
class BinarySer final : public AbstractSerializer {
private:
	Stream& _out;

	virtual std::unique_ptr<AbstractMapSer> _sMap(size_t);
	virtual std::unique_ptr<AbstractSeqSer> _sSeq(size_t);
	virtual std::unique_ptr<AbstractStructSer> _sStruct();

	virtual void _sLong(long long);
	virtual void _sULong(unsigned long long);
	//void sWString(std::wstring);
	virtual void _sString(const std::string&);
	virtual void _sBool(bool);
	virtual void _sChar(char);
public:
	BinarySer(Stream& out) : _out(out) {};
};

class BinaryMapSer final : public AbstractMapSer {
private:
	size_t _size;
	BinarySer& _ser;
	Stream& _out;

	virtual void _sKeyVal(const std::string&, const Writer&);
	virtual void _begin();
	virtual void _finish();
public:
	BinaryMapSer(size_t size, BinarySer& ser, Stream& out);

};

class BinarySeqSer final : public AbstractSeqSer {
private:
	size_t _size;
	Stream& _out;
	BinarySer& _ser;

	virtual void _sNext(const Writer& v);
	virtual void _begin();
	virtual void _finish();
public:
	BinarySeqSer(size_t size, BinarySer& ser, Stream& out);
	~BinarySeqSer();

};

class BinaryStructSer final : public AbstractStructSer {
private:
	Stream& _out;
	BinarySer& _ser;
	virtual void _sKeyVal(const std::string&, const Writer&);
	virtual void _begin();
	virtual void _finish();
public:
	BinaryStructSer(BinarySer& ser, Stream& out);

};



