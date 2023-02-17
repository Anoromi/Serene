#pragma once
#include <string>
#include <vector>
#include "BinarySer.h"
#include "JSONSer.h"
#include "Example.h"



void serTest() {

	{
		std::ofstream outBinary("./outBinary.txt");
		outBinary.clear();
		BinarySer s(outBinary);
		Person p = { "Andrii Zahorulko", 19, 'm', std::vector < std::string> {"c#", "rust", "dart"} };
		write(s, p);
		outBinary.close();
	}

	{
		std::ofstream outJSON("./outJSON.txt");
		outJSON.clear();
		JSONSer s(outJSON);
		Person p = { "Andrii Zahorulko", 19, 'm', std::vector < std::string> {"c#", "rust", "dart"} };
		write(s, p);
		//write(s, p);
		outJSON.close();
	}

	{
		
	}
}
