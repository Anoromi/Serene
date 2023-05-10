#pragma once
#include "Serializer.h"

template<typename T>
	requires std::same_as<std::optional<typename T::value_type>, T>
inline Writer write(const T& v) {
	return[&v](AbstractSerializer& ser) {
		auto st = ser.sStruct();
		st->begin();
		st->sKeyVal("hasValue", write(v.has_value()));
		if (v.has_value()) {
			st->sKeyVal("value", write(*v));
		}

		st->finish();
	};
}


template<typename T>
	requires std::same_as<std::unique_ptr<typename T::element_type>, T>
inline Writer write(const T& v) {
	return [&v](AbstractSerializer& s) {
		write(v.get())(s);
	};
}

template<typename T>
	requires std::same_as<std::shared_ptr<typename T::element_type>, T>
inline Writer write(const T& v) {
	return [&v](AbstractSerializer& s) {
		write(v.get())(s);
	};
}
