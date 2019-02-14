#include "SceneManager.h"
#include "../../AI/Source/Scene.h"

SceneManager::SceneManager() : activeScene(nullptr), nextScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(double _dt)
{
	// Check for change of scene
	if (nextScene != activeScene)
	{
		if (activeScene)
		{
			// Scene is valid, need to call appropriate function to exit
			activeScene->Exit();
		}
		
		activeScene = nextScene;
		activeScene->Init();
	}

	if (activeScene)
		activeScene->Update(_dt);
}

void SceneManager::Render()
{
	if (activeScene)
		activeScene->Render();
}

void SceneManager::Exit()
{
	// Delete all scenes stored and empty the entire map
	activeScene->Exit();
	activeScene = nullptr;
	std::map<std::string, Scene*>::iterator it, end;
	end = sceneMap.end();
	for (it = sceneMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	sceneMap.clear();
}

void SceneManager::AddScene(const std::string& _name, Scene* _scene)
{
	if (CheckSceneExist(_name))
	{
		// Scene Exist, unable to proceed
		throw std::exception("Duplicate scene name provided");
	}

	if (_scene == nullptr)
	{
		throw std::invalid_argument("Invalid scene pointer");
	}

	// Nothing wrong, add the scene to our map
	sceneMap[_name] = _scene;
}

void SceneManager::RemoveScene(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!CheckSceneExist(_name))
		return;

	Scene* target = sceneMap[_name];
	if (target == activeScene || target == nextScene)
	{
		throw std::exception("Unable to remove active/next scene");
	}

	// Delete and remove from our map
	delete target;
	sceneMap.erase(_name);
}

void SceneManager::SetActiveScene(const std::string& _name)
{
	if (!CheckSceneExist(_name))
	{
		// Scene does not exist
		throw std::exception("Scene does not exist");
	}

	// Scene exist, set the next scene pointer to that scene
	nextScene = sceneMap[_name];
}

bool SceneManager::CheckSceneExist(const std::string& _name)
{
	return sceneMap.count(_name) != 0;
}