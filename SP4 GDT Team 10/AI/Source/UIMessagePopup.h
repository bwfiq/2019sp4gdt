#include "UIBase.h"

class UIMessagePopup : public UIBase
{
public:
	UIMessagePopup(const std::string& text, float duration = 4);
	~UIMessagePopup();
	
	float fElapsedTime;
	float fDuration;

	virtual void Update(float dt);
private:
	//Order of ui components matter
	enum UI_STARTBUTTON_COMPONENTS {
		COMPONENT_OUTLINEBAR,

		COMPONENT_GREYBAR,
		COMPONENT_TEXT,

		COMPONENT_TOTAL
	};
};