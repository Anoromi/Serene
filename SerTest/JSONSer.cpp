#include "JSONSer.h"
#include <boost/json.hpp>


void JSONMapSer::_begin()
{
	_out << '{';
}

void JSONMapSer::_finish()
{

	_out << '}';
}

std::unique_ptr<AbstractMapSer> JSONSer::_sMap(size_t)
{
	return  std::unique_ptr<AbstractMapSer>(new JSONMapSer(_out, *this));
}

std::unique_ptr<AbstractSeqSer> JSONSer::_sSeq(size_t)
{
	return std::unique_ptr<AbstractSeqSer>(new JSONSeqSer(_out, *this));
}

std::unique_ptr<AbstractStructSer> JSONSer::_sStruct()
{
	return std::unique_ptr<AbstractStructSer>(new JSONStructSer(_out, *this));
}

void JSONSer::_sLong(long long v)
{
	_out << v;
}


void JSONSer::_sULong(unsigned long long v)
{
	_out << v;
}

void JSONSer::_sString(const std::string& v)
{
	_out << boost::json::serialize(boost::json::string(v));
}

void JSONSer::_sBool(bool v)
{
	_out << v;
}

void JSONSer::_sChar(char v)
{
	_out << "\"" << v << "\"";
}

void JSONSeqSer::_begin()
{
	_out << '[';
}

void JSONSeqSer::_finish()
{
	_out << ']';
}

void JSONStructSer::_begin()
{
	_out << '{';
}

void JSONStructSer::_finish()
{
	_out << '}';
}


void JSONMapSer::_sKeyVal(const std::string& k, const Writer& v)
{
	if (addComma)
		_out << ',';
	_ser.sString(k);
	_out << ':';
	v(_ser);
	addComma = true;
}

void JSONStructSer::_sKeyVal(const std::string& k, const Writer& v)
{
	if (addComma)
		_out << ',';
	_ser.sString(k);
	_out << ':';
	v(_ser);
	addComma = true;
}

void JSONSeqSer::_sNext(const Writer& v)
{
	if (addComma)
		_out << ',';
	v(_ser);
	addComma = true;
}
