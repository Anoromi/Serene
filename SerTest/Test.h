#pragma once
#include <string>
#include <vector>
#include "BinarySer.h"
#include "Deserializer.h"
#include "Example.h"
#include <boost/json.hpp>
#include "JSONSer.h"
#include "JSONDe.h"
#include "BinaryDe.h"

template<typename T>
inline void serTest(std::string path, const T& value) {
	{
		std::ofstream outBinary(path + ".txt");
		outBinary.clear();
		BinarySer s(outBinary);
		write(value)(s);
		outBinary.close();
	}

	{
		std::ofstream outJSON(path + ".json");
		outJSON.clear();
		JSONSer s(outJSON);
		write(value)(s);
		outJSON.close();
	}


	{
		std::ifstream inBinary(path + ".txt");
		BinaryDe de(inBinary);
		DeserialzationScope scope;
		DeserializeBuffer<T> value;
		deserialize(value.buffer())(de, scope);

		auto result = value.operator*();
		std::cout << result << std::endl;
	}

	{
		std::ifstream inJson(path + ".json");
		boost::json::value v(boost::json::parse(inJson));

		JSONDe de(v);
		DeserialzationScope scope;
		DeserializeBuffer<T> value;
		deserialize(value.buffer())(de, scope);

		auto result = value.operator*();
		std::cout << result << std::endl;
	}

}


void serTest1() {

	Person p("Andrii Zahorulko", 19, Sex::male);
	serTest("./out1", p);

	Student s("Andrii Zahorulko", 19, Sex::male, { "dart", "c#", "rust" });
	serTest("./out2", s);

}
