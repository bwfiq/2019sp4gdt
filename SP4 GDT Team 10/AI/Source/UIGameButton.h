#include "UIBase.h"

class UIGameButton : public UIBase
{
public:
	enum BUTTON_TYPE {
		BUTTON_DAILYREQUIREMENT,
		BUTTON_SELECTED_ALTAR_OFFER,

		BUTTON_TOTAL
	};
	UIGameButton(BUTTON_TYPE buttonType);
	~UIGameButton();

	virtual void Update(float dt);
	void SetText(std::string text);
private:
	//Order of ui components matter
	BUTTON_TYPE buttonType;
	enum UI_GAMEBUTTON_COMPONENTS {
		COMPONENT_BOARD,
		COMPONENT_TEXT,

		COMPONENT_TOTAL,
	};
};