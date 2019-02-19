#include "UIBase.h"

class UICoreResources : public UIBase
{
public:
	UICoreResources();
	~UICoreResources();

	virtual void Update(float dt);
private:
	//Order of ui components matter
	enum UI_CORERESOURCES_COMPONENTS {
		COMPONENT_OUTLINEBAR,


		COMPONENT_TOTAL
	};
};