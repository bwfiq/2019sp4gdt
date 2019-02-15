#include "UIBase.h"

class UIReligionBar : public UIBase
{
public:
	UIReligionBar();
	~UIReligionBar();

	virtual void Update(float dt);
private:
	//Order of ui components matter
	enum UI_RELIGIONBAR_COMPONENTS {
		COMPONENT_OUTLINEBAR,

		COMPONENT_GREYBAR,

		//COMPONENT_REDBAR,
		//COMPONENT_YELLOWBAR,
		//COMPONENT_GREENBAR,

		COMPONENT_THEBAR,

		COMPONENT_FIRSTTOP,
		COMPONENT_FIRSTBOT,
		COMPONENT_SECONDTOP,
		COMPONENT_SECONDBOT,


		COMPONENT_TOTAL
	};
};