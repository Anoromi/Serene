#include "BinarySer.h"
#include <bitset>


std::unique_ptr<AbstractMapSer> BinarySer::_sMap(size_t size)
{
	return std::unique_ptr<AbstractMapSer>(new BinaryMapSer(size, *this, _out));
}

std::unique_ptr<AbstractSeqSer> BinarySer::_sSeq(size_t size)
{
	return std::unique_ptr<AbstractSeqSer>(new BinarySeqSer(size, *this, _out));
}

std::unique_ptr<AbstractStructSer> BinarySer::_sStruct()
{
	return std::unique_ptr<AbstractStructSer>(new BinaryStructSer(*this, _out));
}


void BinarySer::_sLong(long long v)
{
	_sULong((unsigned long long) v);
	//_out
	//	<< char(v >> 56) << char(v >> 48) << char(v >> 40) << char(v >> 32) << char(v >> 24) << char(v >> 16) << char(v >> 8) << char(v);
}

void BinarySer::_sULong(unsigned long long v)
{
	long long vCur = v;
	while (vCur > 0x7F) {
		char next = vCur & 0x7F;
		_out << next;
		vCur >>= 7;
	}
	_out << char(0x80 | vCur);
	_out.flush();
	//_out
	//	<< char(v >> 56) << char(v >> 48) << char(v >> 40) << char(v >> 32) << char(v >> 24) << char(v >> 16) << char(v >> 8) << char(v);
}



void BinarySer::_sString(const std::string& v)
{
	sULong(unsigned long long(v.size()));
	_out << v;
}

void BinarySer::_sBool(bool v)
{
	_out << v;
}

void BinarySer::_sChar(char v)
{
	_out << v;
}


BinaryMapSer::BinaryMapSer(size_t size, BinarySer& ser, OutStream& out) : _size(size), _ser(ser), _out(out) {}


void BinaryMapSer::_sKeyVal(const std::string& k, const Writer& f)
{
	if (_size-- == 0)
		throw ExceededSize();
	_ser.sString(k);
	f(_ser);
}

void BinaryMapSer::_begin() {
	_ser.sULong(_size);
}

void BinaryMapSer::_finish()
{
	if (_size != 0)
		throw _size;
}

BinarySeqSer::BinarySeqSer(size_t size, BinarySer& ser, OutStream& out)
	: _size(size), _ser(ser), _out(out) {}

BinarySeqSer::~BinarySeqSer()
{
	if (_size != 0)
		throw _size;
}

void BinarySeqSer::_begin()
{
	_ser.sULong((unsigned long)_size);
}

void BinarySeqSer::_finish()
{
	if (_size != 0)
		throw _size;
}

void BinarySeqSer::_sNext(const Writer& t)
{
	if (_size-- == 0)
		throw ExceededSize();
	t(_ser);
}


void BinaryStructSer::_finish() {}
void BinaryStructSer::_begin() {}

inline void BinaryStructSer::_sKeyVal(const std::string&, const Writer& t)
{
	t(_ser);
}

BinaryStructSer::BinaryStructSer(BinarySer& ser, OutStream& out) : _ser(ser), _out(out) {}

