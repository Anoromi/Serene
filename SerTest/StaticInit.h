#pragma once

template<void Func()>
class StaticInit {
public:
	StaticInit() {
		Func();
	}
};
