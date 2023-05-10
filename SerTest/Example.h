#pragma once
#include <string>
#include "Serializer.h"
#include "Deserializer.h"
#include "DeserializeExt.h"

enum Sex {
	male, female
};


Writer write(const std::same_as<Sex> auto& v) {
	return [&v](AbstractSerializer& s) {
		switch (v)
		{
		case male:
			s.sString("male");
			break;
		case female:
			s.sString("female");
			break;
		}
	};
}

template<typename T>
	requires std::same_as<T, Sex>
Deserialize read(BufferAlloc& v) {
	return [&v](AbstractDeserializer& s, DeserialzationScope&) {
		std::string value = s.dString();

		if (value == "male") {
			new (v.getBuffer()) Sex(male);
		}
		else if (value == "female") {
			new (v.getBuffer()) Sex(female);
		}
		else {
			throw DeserializationException("Expected male or female string");
		}
	};
}



class Person : public SerializableObject {
	RegisterSerializeClass(Person);
private:
	std::string _name;
	unsigned int _age;
	Sex _sex;
private:
	Writer _serialize() const override { return serializeCurrent(); }
public:
	Person(const std::string& name, const unsigned int& age, const Sex& sex)
		: _name(name), _age(age), _sex(sex) {
	}

	virtual ~Person() { }

	Writer serializeCurrent() const;
	static Deserialize read(BufferAlloc& buffer);

	void show(std::ostream&);

	inline const std::string& getName() const { return _name; }
	inline unsigned int getAge() const { return _age; }
	inline Sex getSex() const { return _sex; }
};


inline std::ostream& operator<<(std::ostream& o, Person& p) {
	p.show(o);
	return o;
}

class Student : public Person {
	RegisterSerializeClass(Student);
private:
	std::vector<std::string> _languages;

	Writer _serialize() const override { return serializeCurrent(); }

public:
	Student(const Person& person, const std::vector<std::string>& languages)
		: Person(person), _languages(languages) {
	}

	Student(const std::string& name, const unsigned int& age, const Sex& sex, const std::vector<std::string>& languages)
		: Person(name, age, sex), _languages(languages) {
	}

	Writer serializeCurrent() const;
	static Deserialize read(BufferAlloc& buffer);
	void show(std::ostream&);

};


inline std::ostream& operator<<(std::ostream& o, Student& p) {
	p.show(o);
	return o;
}


class Color : public SerializableObject {
	RegisterSerializeClass(Color);
private:
	char _red;
	char _green;
	char _blue;
	Writer _serialize() const override { return serilaizeCurrent(); }

public:
	Color(char red, char green, char blue) : _red(red), _green(green), _blue(blue) {}
	static Deserialize read(BufferAlloc& buffer);
	Writer serilaizeCurrent() const;
};



template<Serializable T>
class BinaryTree {
private:
	struct Node {
		T _value;
		Node* _left;
		Node* _right;
		Node(const T& value, Node* _left = nullptr, Node* _right = nullptr) : _value(value), _left(_left), _right(_right) {}
	};
	Node* _root;

public:

	BinaryTree() : _root(nullptr) {};

	void add(const T&);
};

template<Serializable T>
inline void BinaryTree<T>::add(const T& v)
{
	if (_root == nullptr)
		_root = new Node(v);
	Node* current;
	while (true) {
		if (v < current->_value && current->_left) {
			current = current->_left;
		}
		else if (v > current->_value && current->_right) {
			current = current->_right;
		}
		else {
			return;
		}
	}
	if (v < current->_value) {
		current->_left = new Node(v);
	}
	else if (v > current->_value) {
		current->_right = new Node(v);
	}
}

