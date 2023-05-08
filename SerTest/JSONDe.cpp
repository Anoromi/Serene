#include "JSONDe.h"
#include <iostream>

long long JSONDe::_dLong()
{
	return _value.as_int64();
}

unsigned long long JSONDe::_dULong()
{
	//std::cout << std::boolalpha << _value.is_int64() << _value.is_uint64() << _value.as_int64();
	if (_value.is_int64())
		return _value.as_int64();
	else
		return _value.as_uint64();
}

std::string JSONDe::_dString()
{
	return std::string(_value.as_string().operator std::string_view());
}

bool JSONDe::_dBool()
{
	return _value.as_bool();
}

char JSONDe::_dChar()
{
	return _value.as_string()[0];
}

std::unique_ptr<AbstractMapDe> JSONDe::_dMap()
{
	return std::unique_ptr<AbstractMapDe>(new JSONMapDe(_value.as_object()));
}

std::unique_ptr<AbstractSeqDe> JSONDe::_dSeq()
{
	return std::unique_ptr<AbstractSeqDe>(new JSONSeqDe(_value.as_array()));
}

std::unique_ptr<AbstractStructDe> JSONDe::_dStruct()
{
	return std::unique_ptr<AbstractStructDe>(new JSONStructDe(_value.as_object()));
}

bool JSONMapDe::_hasMore()
{
	return _current != _last;
}

std::string JSONMapDe::_nextKey()
{
	boost::json::string_view s = _current->key();
	_currentValue = &_current->value();
	return s;
}



void JSONMapDe::_nextValue(DeserialzationScope& scope, const Deserialize& d)
{
	if (_currentValue == nullptr) {
		throw DeserializationException("Key/Value pair hasn't been read yet");
	}
	JSONDe deserializer = JSONDe(*_currentValue);
	d(deserializer, scope);
	++_current;
}

void JSONMapDe::_begin()
{
}

void JSONMapDe::_end()
{
}

bool JSONSeqDe::_hasMore()
{
	return _current != _last;
}

void JSONSeqDe::_nextItem(DeserialzationScope& scope, const Deserialize& d)
{
	JSONDe des(*_current);
	d(des, scope);
	++_current;
}

void JSONSeqDe::_begin()
{
}

void JSONSeqDe::_end()
{
}

void JSONStructDe::_next(DeserialzationScope& scope, const std::string& key, const Deserialize& d)
{
	JSONDe des(_obj.at(key));
	d(des, scope);
}

void JSONStructDe::_begin()
{
}

void JSONStructDe::_end()
{
}
