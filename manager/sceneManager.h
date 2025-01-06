#pragma once
#include <list>
#include "main/main.h"

// ‘O•ûéŒ¾
class Scene;

class SceneManager final
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static Scene* m_LoadScene;
	static bool m_LoadFinish;
public:

	static void Init();
	static void Uninit();
	static void Update(const float& deltaTime);
	static void Draw();

	static void ChangeScene();

	template<typename T>
	static T* GetScene()
	{ 
		T* scene = dynamic_cast<T*>(m_Scene);
		return scene;
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