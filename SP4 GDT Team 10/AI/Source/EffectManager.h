#pragma once
#include "SingletonTemplate.h"
#include <vector>

class EffectBase;
class SceneBase;
class Mesh;

class EffectManager : public Singleton<EffectManager>
{
	friend Singleton<EffectManager>;
public:
	void Init();
	void Update(float dt);
	void Render(SceneBase* scene);
	
	bool AddEffect(EffectBase* effect);

	//void SetScene(SceneBase* scene) { this->scene = scene; }
private:
	EffectManager();
	~EffectManager();

	void rendermesh(SceneBase* scene, EffectBase* effect);

	//SceneBase* scene;
	std::vector<EffectBase*> effect_list;
};

