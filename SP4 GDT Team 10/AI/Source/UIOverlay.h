#include "UIBase.h"

class UIOverlay : public UIBase
{
public:
	UIOverlay(const std::string& text, float x = 0, float y = 0);
	~UIOverlay();

	virtual void Update(float dt);
private:
	//Order of ui components matter
	enum UI_STARTBUTTON_COMPONENTS {
		COMPONENT_OVERLAY,

		COMPONENT_TOTAL
	};
};