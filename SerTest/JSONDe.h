#pragma once
#include "Deserializer.h"
#include <fstream>
#include <optional>
#include <boost/json.hpp>
#include <boost/json/detail/value_from.hpp>

class JSONDe final : public AbstractDeserializer {

private:
	boost::json::value& _value;

	virtual long long _dLong();
	virtual unsigned long long _dULong();
	virtual std::string _dString();
	virtual bool _dBool();
	virtual char _dChar();
	virtual std::unique_ptr<AbstractMapDe> _dMap();
	virtual std::unique_ptr<AbstractSeqDe> _dSeq();
	virtual std::unique_ptr<AbstractStructDe> _dStruct();
public:
	JSONDe( boost::json::value& value) : _value(value) {}; 
};

class JSONMapDe final : public AbstractMapDe {
private:
	boost::json::object::iterator _current;
	boost::json::object::iterator _last;

	 boost::json::value* _currentValue = nullptr;
	
	virtual bool _hasMore() override;
	virtual std::string _nextKey() override;
	virtual void _nextValue(DeserialzationScope& scope, const Deserialize&) override;
	virtual void _begin() override;
	virtual void _end() override;
public:
	JSONMapDe(boost::json::object& map) : _current(map.begin()), _last(map.end()) {}

};


class JSONSeqDe final : public AbstractSeqDe {
private:
	boost::json::array::iterator _current;
	boost::json::array::iterator _last;

	virtual bool _hasMore();
	virtual void _nextItem(DeserialzationScope&, const Deserialize&);
	virtual void _begin() override;
	virtual void _end() override;
public:
	JSONSeqDe(boost::json::array& arr) : _current(arr.begin()), _last(arr.end()) {}

};

class JSONStructDe final : public AbstractStructDe {
private:
	boost::json::object& _obj;
	virtual void _next(DeserialzationScope& scope, const std::string&, const Deserialize&);
	virtual void _begin() override;
	virtual void _end() override;
public:
	JSONStructDe(boost::json::object& obj) : _obj(obj) {}

};
