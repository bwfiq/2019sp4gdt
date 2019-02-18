#include "UIBase.h"

class UIAltarPopup : public UIBase
{
public:
	UIAltarPopup();
	~UIAltarPopup();

	virtual void Update(float dt);
private:
	//Order of ui components matter
	enum UI_STARTBUTTON_COMPONENTS {
		COMPONENT_OUTLINEBAR,

		COMPONENT_GREYBAR,

		COMPONENT_TOTAL
	};
	bool bCursorVisibleThing;
};