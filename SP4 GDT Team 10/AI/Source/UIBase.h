#pragma once
#include "Vector3.h"
#include <vector>
class Mesh;

class UIBase {
public:
	UIBase();
	virtual ~UIBase();

	std::vector<Mesh*> meshes;

	Vector3 pos;
	Vector3 scale;
	Vector3 anchorPoint;
	bool bLightEnabled;
private:
	virtual void Render() = 0;
};