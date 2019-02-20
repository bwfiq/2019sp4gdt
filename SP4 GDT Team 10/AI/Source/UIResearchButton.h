#include "UIBase.h"

class UIResearchButton : public UIBase
{
public:
	UIResearchButton(const std::string& text, float x = 0, float y = 0);
	~UIResearchButton();

	virtual void Update(float dt);
private:
	//Order of ui components matter
	enum UI_STARTBUTTON_COMPONENTS {
		COMPONENT_OUTLINEBAR,

		COMPONENT_GREYBAR,

		COMPONENT_TOTAL
	};
};