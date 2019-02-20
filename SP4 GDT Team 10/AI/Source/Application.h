
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

struct Scene;
struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);
	static void SetMouseVisiblity(bool isVisible);

	void UpdateInput();
	void PostInputUpdate();
	void QuitGame();

	void Iterate();

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
	Scene* temp;
	bool quit = false;
};

#endif