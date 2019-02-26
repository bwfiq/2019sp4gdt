#pragma once
#include <map>
#include <string>

struct Vector3;
class UIBase;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! [ CURRENTLY SUPPORTS POSITION AND SCALING TWEEN ] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 

class UITween {
public:
	enum UI_EASINGSTYLE {
		ES_LINEAR,
		ES_CUBIC,
		ES_SINE,
		ES_BACK,

		ES_TOTAL
	};
	enum UI_EASINGDIRECTION {
		ED_OUT,
		ED_IN,
		ED_INOUT,

		ED_TOTAL
	};
	UITween(
		UIBase* ui,
		float duration,
		UI_EASINGSTYLE easingStyle = ES_LINEAR,
		UI_EASINGDIRECTION easingDirection = ED_OUT,
		int repeatCount = 0,
		//bool reverses = false,
		float delayTime = 0
	);
	~UITween();

	bool IsDone();
	void Update(float dt);

	std::map<std::string, Vector3> properties_goal;
protected:
	UIBase* ui;
	float fElapsedTime;
	float fDuration;
	int iRepeatCount, iRepeatRequired;
	//bool bReverses;
	UI_EASINGSTYLE easingStyle;
	UI_EASINGDIRECTION easingDirection;
	std::map<std::string, Vector3> properties_start;
	bool bIsDone;

	float(*easingFunction)(float, float, float, float);
private:
};