#include "UIBase.h"

class UIGameText : public UIBase
{
public:
	enum TEXT_TYPE {
		TEXT_DAILYREQUIREMENT,

		TEXT_TOTAL
	};
	UIGameText(TEXT_TYPE textType);
	~UIGameText();

	virtual void Update(float dt);
	void SetText(std::string text);
private:
	//Order of ui components matter
	TEXT_TYPE textType;
	enum UI_GAMEBUTTON_COMPONENTS {
		COMPONENT_BOARD,
		COMPONENT_TEXT_1,
		COMPONENT_TEXT_2,
		COMPONENT_TEXT_3,

		COMPONENT_TOTAL,
	};
};