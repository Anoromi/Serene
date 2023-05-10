#pragma once
#include "Serializer.h"
#include <fstream>
#include <memory>
#include "types.h"

class JSONSer;
class JSONMapSer;
class JSONSeqSer;
class JSONStructSer;

class JSONSer final
	: public AbstractSerializer {
private:
	OutStream& _out;

	virtual	std::unique_ptr<AbstractMapSer> _sMap(size_t);
	virtual std::unique_ptr<AbstractSeqSer> _sSeq(size_t);
	virtual std::unique_ptr<AbstractStructSer> _sStruct();
	virtual void _sLong(long long);
	virtual void _sULong(unsigned long long);
	virtual void _sString(const std::string& v);
	virtual void _sBool(bool);
	virtual void _sChar(char);
public:
	JSONSer(OutStream& out) : _out(out) {
		out << std::boolalpha;
	};
};

class JSONMapSer final : public AbstractMapSer {
private:
	OutStream& _out;
	JSONSer& _ser;
	bool addComma = false;

	virtual void _sKeyVal(const std::string&, const Writer& v);
	virtual void _begin();
	virtual void _finish();
public:
	JSONMapSer(OutStream& out, JSONSer& ser) : _out(out), _ser(ser) { }
	~JSONMapSer() { }


};

class JSONSeqSer final : public AbstractSeqSer {
private:
	OutStream& _out;
	JSONSer& _ser;
	bool addComma = false;
	virtual void _sNext(const Writer& v);
	virtual void _begin();
	virtual void _finish();

public:
	JSONSeqSer(OutStream& out, JSONSer& ser) : _out(out), _ser(ser) { }
	~JSONSeqSer() { }

};

class JSONStructSer final : public AbstractStructSer {
private:
	OutStream& _out;
	JSONSer& _ser;
	bool addComma = false;
	virtual void _sKeyVal(const std::string&, const Writer&);
	virtual void _begin();
	virtual void _finish();
public:
	JSONStructSer(OutStream& out, JSONSer& ser) : _out(out), _ser(ser) { }
	~JSONStructSer() { }

};

