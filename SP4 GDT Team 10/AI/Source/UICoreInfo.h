#include "UIBase.h"

struct Vector3;

class UICoreInfo : public UIBase
{
public:
	enum INFO_TYPE {
		INFO_DAY,
		INFO_FOOD,
		INFO_POPULATION,
		INFO_STONE,
		INFO_TIME,
		INFO_WOOD,

		INFO_TOTAL
	};
	UICoreInfo(INFO_TYPE infoType, Vector3 origPos);
	~UICoreInfo();

	virtual void Update(float dt);
private:
	//Order of ui components matter
	INFO_TYPE infoType;
	Vector3 origPos;
	enum UI_COREINFO_COMPONENTS {
		COMPONENT_BOARD,
		COMPONENT_TEXT,

		COMPONENT_TOTAL
	};
};