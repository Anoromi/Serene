#pragma once
#include "Serializer.h"
#include <iostream>
#include <fstream>
#include "types.h"

class BinarySer;
class BinaryMapSer;
class BinarySeqSer;
class BinaryStructSer;


class BinarySer final : public AbstractSerializer {
private:
	OutStream& _out;

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
	BinarySer(OutStream& out) : _out(out) {};
};

class BinaryMapSer final : public AbstractMapSer {
private:
	size_t _size;
	BinarySer& _ser;
	OutStream& _out;

	virtual void _sKeyVal(const std::string&, const Writer&);
	virtual void _begin();
	virtual void _finish();
public:
	BinaryMapSer(size_t size, BinarySer& ser, OutStream& out);

};

class BinarySeqSer final : public AbstractSeqSer {
private:
	size_t _size;
	OutStream& _out;
	BinarySer& _ser;

	virtual void _sNext(const Writer& v);
	virtual void _begin();
	virtual void _finish();
public:
	BinarySeqSer(size_t size, BinarySer& ser, OutStream& out);
	~BinarySeqSer();

};

class BinaryStructSer final : public AbstractStructSer {
private:
	OutStream& _out;
	BinarySer& _ser;
	virtual void _sKeyVal(const std::string&, const Writer&);
	virtual void _begin();
	virtual void _finish();
public:
	BinaryStructSer(BinarySer& ser, OutStream& out);

};



