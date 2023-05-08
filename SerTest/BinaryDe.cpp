#include "BinaryDe.h"

long long BinaryDe::_dLong()
{
	long long v = 0;

	for (size_t i = 0; i < 8; i++)
	{
		char next;
		_in >> next;
		v = (v + next) << 8;
	}
	return v;
}

unsigned long long BinaryDe::_dULong()
{
	unsigned long long v = 0;

	for (size_t i = 0; i < 8; i++)
	{
		char next;
		_in >> next;
		v = (v << 8) + next;
	}
	return v;
}

std::string BinaryDe::_dString()
{

	unsigned long long size = dULong();
	char* c = new char[size];
	_in.read(c, size);
	std::string s(c, size);
	delete[] c;
	return s;
}

bool BinaryDe::_dBool()
{
	bool b;
	_in >> b;
	return b;
}

char BinaryDe::_dChar()
{
	char c;
	_in >> c;
	return c;
}

std::unique_ptr<AbstractMapDe> BinaryDe::_dMap()
{
	return std::unique_ptr<AbstractMapDe>(new BinaryMapDe(_in, *this));
}

std::unique_ptr<AbstractSeqDe> BinaryDe::_dSeq()
{
	return std::unique_ptr<AbstractSeqDe>(new BinarySeqDe(_in, *this));
}

std::unique_ptr<AbstractStructDe> BinaryDe::_dStruct()
{
	return std::unique_ptr<AbstractStructDe>(new BinaryStructDe(*this));
}

bool BinaryMapDe::_hasMore()
{
	return _size > 0;
}

std::string BinaryMapDe::_nextKey()
{
	return _de.dString();
}

void BinaryMapDe::_nextValue(DeserialzationScope& scope, const Deserialize& deserialize)
{
	deserialize(_de, scope);
	_size--;
}

void BinaryMapDe::_begin()
{
	_size = _de.dULong();
}

void BinaryMapDe::_end()
{

}

bool BinarySeqDe::_hasMore()
{
	return _size > 0;
}

void BinarySeqDe::_nextItem(DeserialzationScope& scope, const Deserialize& deserialize)
{
	deserialize(_de, scope);
	--_size;
}

void BinarySeqDe::_begin()
{
	_size = _de.dULong();
}

void BinarySeqDe::_end()
{
}

void BinaryStructDe::_next(DeserialzationScope& scope, const std::string&, const Deserialize& deserialize)
{
	deserialize(_de, scope);
}

void BinaryStructDe::_begin()
{
}

void BinaryStructDe::_end()
{
}
