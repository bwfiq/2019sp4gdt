#pragma once
#include "SingletonTemplate.h"
#include <vector>
#include <queue>
#include "Vector3.h"

class EffectBase;
class SceneBase;
class Mesh;

class EffectManager : public Singleton<EffectManager>
{
	friend Singleton<EffectManager>;
public:
	enum EFFECT_PREFABS {
		PREFAB_PLACEOBJECT,
		PREFAB_PLACEVILLAGER,

		PREFAB_COMPLETEOBJECT,

		PREFAB_EARTHQUAKE_DEBRIS,

		PREFAB_TOTAL
	};
	void Init();
	void Update(float dt);
	void Render(SceneBase* scene);
	
	bool AddEffect(EffectBase* effect);

	//uhh yeah its like it auto does a thingo
	void DoPrefabEffect(EFFECT_PREFABS prefab, Vector3 goPos = Vector3());
private:
	EffectManager();
	~EffectManager();

	void rendermesh(SceneBase* scene, EffectBase* effect);

	//SceneBase* scene;
	std::vector<EffectBase*> effect_list;
	std::queue<EffectBase*> effect_queue;//this gon b used to uhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh add effects WITHOUT breaking the Update (via iterator thingo ya watever)
};

