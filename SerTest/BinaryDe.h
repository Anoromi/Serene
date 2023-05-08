#pragma once
#include "Deserializer.h"
#include <fstream>

using IStream = std::ifstream;
class BinaryDe final : public AbstractDeserializer {
private:
	IStream& _in;
	virtual long long _dLong() override;
	virtual unsigned long long _dULong() override;
	virtual std::string _dString() override;
	virtual bool _dBool() override;
	virtual char _dChar() override;
	virtual std::unique_ptr<AbstractMapDe> _dMap() override;
	virtual std::unique_ptr<AbstractSeqDe> _dSeq() override;
	virtual std::unique_ptr<AbstractStructDe> _dStruct() override;

public:
	BinaryDe(IStream& in) : _in(in) {}
};

class BinaryMapDe final : public AbstractMapDe {
private:
	IStream& _in;
	BinaryDe& _de;
	size_t _size = 0;
	virtual bool _hasMore() override;
	virtual std::string _nextKey() override;
	virtual void _nextValue(DeserialzationScope&, const Deserialize&) override;
	virtual void _begin() override;
	virtual void _end() override;
public:
	BinaryMapDe(IStream& in, BinaryDe& de) : _in(in), _de(de) {}
};


class BinarySeqDe final : public AbstractSeqDe {
private:
	IStream& _in;
	BinaryDe& _de;
	size_t _size = 0;
	virtual bool _hasMore() override;
	virtual void _nextItem(DeserialzationScope&, const Deserialize&) override;
	virtual void _begin() override;
	virtual void _end() override;
public:
	BinarySeqDe(IStream& in, BinaryDe& de) : _in(in), _de(de) {}
};

class BinaryStructDe final : public AbstractStructDe {
private:
	BinaryDe& _de;
	virtual void _next(DeserialzationScope& scope, const std::string&, const Deserialize&) override;
	virtual void _begin() override;
	virtual void _end() override;
public:
	BinaryStructDe(BinaryDe& de) : _de(de) {}

};
