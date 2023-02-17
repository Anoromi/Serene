#include "JSONSer.h"

void JSONSer::sBool(bool v)
{
	_out << std::boolalpha << v;
}

void JSONSer::sChar(char v)
{
	_out << "\"" << v << "\"";
}

void JSONMapSer::start()
{
	_out << '{';
}

void JSONMapSer::finish()
{

	_out << '}';
}

JSONMapSer JSONSer::sMap(size_t)
{
	return JSONMapSer(_out, *this);
}

JSONSeqSer JSONSer::sSeq(size_t)
{
	return JSONSeqSer(_out, *this);
}

JSONStructSer JSONSer::sStruct()
{
	return JSONStructSer(_out, *this);
}

void JSONSer::sLong(long long v)
{
	_out << v;
}

void JSONSer::sULong(unsigned long long v)
{
	_out << v;
}

void JSONSer::sString(const std::string& v)
{
	_out << "\"" << v << "\"";
}

void JSONSeqSer::begin()
{
	_out << '[';
}

void JSONSeqSer::finish()
{
	_out << ']';
}

void JSONStructSer::begin()
{
	_out << '{';
}

void JSONStructSer::finish()
{
	_out << '}';
}
