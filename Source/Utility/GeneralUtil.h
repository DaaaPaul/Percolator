#pragma once

#define DEFINE_CALLED(funcName) \
	class funcName##Called { \
		funcName##Called() = default; \
		friend funcName##Called funcName(); \
	};