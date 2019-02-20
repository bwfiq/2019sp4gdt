#pragma once
#include "MyMath.h"

namespace EasingStyle
{
	inline float easeInSine(float t, float b, float c, float d) {
		return -c * cosf(t / d * (Math::PI / 2)) + c + b;
	};
	inline float easeOutSine(float t, float b, float c, float d) {
		return c * sinf(t / d * (Math::PI / 2)) + b;
	};
	inline float easeInOutSine(float t, float b, float c, float d) {
		return -c / 2 * (cosf(Math::PI*t / d) - 1) + b;
	};
}