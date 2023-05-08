
#include "Deserializer.h"

 void* DeserialzationScope::read(const std::string& k) const
{

	for (size_t i = scope.size(); i >= 1; i--) {
		if (scope[i - 1].contains(k)) {
			return scope[i - 1].at(k);
		}
	}
	return nullptr;
}
