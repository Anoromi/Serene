#pragma once
#include <concepts>


class BufferAlloc {
private:
	virtual char* _getBuffer() = 0;
public:
	virtual ~BufferAlloc() {};
	inline char* getBuffer() { return _getBuffer(); }
};

template<typename T>
class StackAllocBuffer : public BufferAlloc {
private:
	char _buffer[sizeof(T)];
	bool _wasInitialized = false;

	virtual char* _getBuffer() {
		return _buffer;
	}

public:
	virtual ~StackAllocBuffer() {
		if (_wasInitialized)
			reinterpret_cast<T*>(_buffer)->~T();
	};

	inline T* getValue() {
		return reinterpret_cast<T*>(getBuffer());
	}

	T&& operator*() {
		return std::move(*reinterpret_cast<T*>(getBuffer()));
	}

	void setWasInitialized(bool wasInitialized) { _wasInitialized = wasInitialized; }
};

class HeapAllocBuffer : public BufferAlloc {
private:
	char* _buffer;

	virtual char* _getBuffer() {
		return _buffer;
	}
public:
	HeapAllocBuffer(size_t bufferSize) : _buffer(new char[bufferSize]) {}
};
