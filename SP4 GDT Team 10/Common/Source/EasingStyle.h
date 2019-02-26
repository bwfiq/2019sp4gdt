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
	inline float easeInBack(float t, float b, float c, float d) {
		float s = 1.70158f;
		float postFix = t /= d;
		return c * (postFix)*t*((s + 1)*t - s) + b;
	};
	inline float easeOutBack(float t, float b, float c, float d) {
		float s = 1.70158f;
		return c * ((t = t / d - 1)*t*((s + 1)*t + s) + 1) + b;
	};

	inline float easeInOutBack(float t, float b, float c, float d) {
		float s = 1.70158f;
		if ((t /= d / 2) < 1) return c / 2 * (t*t*(((s *= (1.525f)) + 1)*t - s)) + b;
		float postFix = t -= 2;
		return c / 2 * ((postFix)*t*(((s *= (1.525f)) + 1)*t + s) + 2) + b;
	};

	inline float easeInCubic(float t, float b, float c, float d) {
		return c * (t /= d)*t*t + b;
	};
	inline float easeOutCubic(float t, float b, float c, float d) {
		return c * ((t = t / d - 1)*t*t + 1) + b;
	};

	inline float easeInOutCubic(float t, float b, float c, float d) {
		if ((t /= d / 2) < 1) return c / 2 * t*t*t + b;
		return c / 2 * ((t -= 2)*t*t + 2) + b;
	};
}