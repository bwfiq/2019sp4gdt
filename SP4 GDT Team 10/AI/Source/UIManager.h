#pragma once
#include "SingletonTemplate.h"
#include <vector>

class UIBase;
class SceneBase;
class Mesh;
struct Color;
struct Vector3;

class UIManager : public Singleton<UIManager>
{
	friend Singleton<UIManager>;
public:
	void Init();
	void Update(float dt);
	void Render(SceneBase* scene);
	
	bool AddUI(UIBase* ui);

	//void SetScene(SceneBase* scene) { this->scene = scene; }
private:
	UIManager();
	~UIManager();

	void rendermesh(SceneBase* scene, Mesh* mesh, bool bLightEnabled, float alpha);
	void rendertext(SceneBase* scene, Mesh* mesh, std::string text, Color color, bool bLightEnabled, Vector3 pos, float size, float alpha);

	//SceneBase* scene;
	std::vector<UIBase*> ui_list;
};

