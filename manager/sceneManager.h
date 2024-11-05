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
public:

	static void Init();
	static void Uninit();
	static void Update(const float& deltaTime);
	static void Draw();

	template<typename T>
	static T* GetScene()
	{ 
		T* Scene = dynamic_cast<T*>(m_Scene);
		return Scene;
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