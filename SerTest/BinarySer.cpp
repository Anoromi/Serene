#include "BinarySer.h"
#include <bitset>


BinaryMapSer BinarySer::sMap(size_t size)
{
	return BinaryMapSer(size, *this, _out);
}

BinarySeqSer BinarySer::sSeq(size_t size)
{
	return BinarySeqSer(size, *this, _out);
}

BinaryStructSer BinarySer::sStruct()
{
	return BinaryStructSer(*this, _out);
}


void BinarySer::sLong(long long v)
{
	_out << char(v) << char(v >> 8) << char(v >> 16) << char(v >> 24) << char(v >> 32) << char(v >> 40) << char(v >> 48) << char(v >> 56);
}

void BinarySer::sULong(unsigned long long v)
{
	_out << char(v) << char(v >> 8) << char(v >> 16) << char(v >> 24) << char(v >> 32) << char(v >> 40) << char(v >> 48) << char(v >> 56);
}



void BinarySer::sString(const std::string& v)
{
	sULong(unsigned long(v.size() - 1));
	_out << v;
}

void BinarySer::sBool(bool v)
{
	_out << v;
}

void BinarySer::sChar(char v)
{
	_out << v;
}

BinarySeqSer::BinarySeqSer(size_t size, BinarySer& ser, Stream& out)
	: _size(size), _ser(ser), _out(out) {
	_ser.sULong((unsigned long)size);
}

BinarySeqSer::~BinarySeqSer()
{
	if (_size != 0)
		throw _size;
}

void BinarySeqSer::finish()
{
	if (_size != 0)
		throw _size;
}

BinaryMapSer::BinaryMapSer(size_t size, BinarySer& ser, Stream& out) : _size(size), _ser(ser), _out(out) {}


void BinaryMapSer::finish()
{
	if (_size != 0)
		throw _size;
}

BinaryStructSer::BinaryStructSer(BinarySer& ser, Stream& out) : _ser(ser), _out(out) {}

void BinaryStructSer::finish() {}
