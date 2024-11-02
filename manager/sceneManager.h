#pragma once
#include <list>
#include "main/main.h"

class Scene;

class SceneManager final
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() 
	{ 
		return m_Scene; 
	}

	template<typename T>
	static void SetScene()
	{
		if (m_NextScene == nullptr)
		{
			m_NextScene = new T;
		}
	}
};