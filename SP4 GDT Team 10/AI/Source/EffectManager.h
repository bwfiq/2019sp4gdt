#pragma once
#include "SingletonTemplate.h"
#include <vector>
#include <queue>
#include "Vector3.h"
#include "Camera.h"

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
		PREFAB_BLIZZARD_CLOUDS,

		PREFAB_VILLAGER_PANIC,
		PREFAB_VILLAGER_DIE,

		PREFAB_TOTAL
	};
	void Init();
	void Update(float dt);
	void Render(SceneBase* scene);
	
	bool AddEffect(EffectBase* effect);
	void SetCamera(Camera* camera);

	//uhh yeah its like it auto does a thingo
	void DoPrefabEffect(EFFECT_PREFABS prefab, Vector3 goPos = Vector3());
private:
	EffectManager();
	~EffectManager();

	void rendermesh(SceneBase* scene, EffectBase* effect);

	//SceneBase* scene;
	Camera* camera;
	std::vector<EffectBase*> effect_list;
	std::queue<EffectBase*> effect_queue;//this gon b used to uhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh add effects WITHOUT breaking the Update (via iterator thingo ya watever)
};

