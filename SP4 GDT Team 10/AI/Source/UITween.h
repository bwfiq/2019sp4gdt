#pragma once
#include <map>
#include <string>

struct Vector3;
class UIBase;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! [ CURRENTLY SUPPORTS POSITION AND SCALING TWEEN ] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 

class UITween {
public:
	enum TWEEN_EASINGSTYLE {
		ES_LINEAR,
		ES_CUBIC,
		ES_SINE,
		ES_BACK,

		ES_TOTAL
	};
	enum TWEEN_EASINGDIRECTION {
		ED_OUT,
		ED_IN,
		ED_INOUT,

		ED_TOTAL
	};
	enum TWEEN_CALLBACK {
		CALLBACK_NONE,
		CALLBACK_SET_UI_DONE,

		CALLBACK_TOTAL
	};
	UITween(
		UIBase* ui,
		float duration,
		TWEEN_EASINGSTYLE easingStyle = ES_LINEAR,
		TWEEN_EASINGDIRECTION easingDirection = ED_OUT,
		int repeatCount = 0,
		//bool reverses = false,
		float delayTime = 0,
		TWEEN_CALLBACK callBack = CALLBACK_NONE
	);
	~UITween();

	void SetElapsedTime(float time);
	bool IsDone();
	void SetIsDone(bool done);
	void Update(float dt);

	std::map<std::string, Vector3> properties_goal;
protected:
	UIBase* ui;
	float fElapsedTime;
	float fDuration;
	int iRepeatCount, iRepeatRequired;
	//bool bReverses;
	TWEEN_EASINGSTYLE easingStyle;
	TWEEN_EASINGDIRECTION easingDirection;
	TWEEN_CALLBACK callBack;
	std::map<std::string, Vector3> properties_start;
	bool bIsDone;

	float(*easingFunction)(float, float, float, float);

	void TweenCallback();
private:
};