#include "UIBase.h"

class UIReligionBar : public UIBase
{
public:
	UIReligionBar();
	~UIReligionBar();

	virtual void Update(float dt);
private:
	enum UI_RELIGIONBAR_COMPONENTS {
		COMPONENT_OUTLINEBAR,

		COMPONENT_REDBAR,
		COMPONENT_GREYBAR,
		COMPONENT_GREENBAR,

		COMPONENT_BLUETRIANGLE,


		COMPONENT_TOTAL
	};
};