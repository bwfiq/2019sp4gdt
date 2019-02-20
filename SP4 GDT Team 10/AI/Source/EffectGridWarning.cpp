#include "EffectGridWarning.h"
#include "Mesh.h"
#include "SceneData.h"
#include "MouseController.h"
#include "MousePicker.h"
#include "GameObject.h"

EffectGridWarning::EffectGridWarning(Vector3 pos, Vector3 size, float alpha) :
	EffectBase()
{
	SceneData* SD = SceneData::GetInstance();
	mesh = SD->GetMesh("redquad");
	this->pos = pos;
	this->scale = this->origScale = size;
	this->fAlpha = this->origAlpha = alpha;
	this->fElapsedTime = (pos.x + pos.z) * 0.25f;
	this->fDuration = this->fElapsedTime + 20.f;
	rotation.Set(-90, 0, 0);
}
EffectGridWarning::~EffectGridWarning()
{
}

void EffectGridWarning::Update(float dt)
{
	fElapsedTime += dt;
	SceneData* SD = SceneData::GetInstance();
	this->scale = origScale + Vector3(0.25f, 0.25f, 0) * cosf(fElapsedTime * 8);
	this->fAlpha = origAlpha + 0.2f * cosf(fElapsedTime * 8);
	if (fElapsedTime > fDuration)
	{
		this->bIsDone = true;
	}
}
