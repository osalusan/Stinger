#pragma once
#include <list>
#include "main/main.h"

// ‘O•ûéŒ¾
class Scene;
class LoadScene;
class Fade;

class SceneManager final
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static LoadScene* m_LoadScene;
	static Fade* m_Fade;
	static bool m_LoadFinish;
public:

	static void Init();
	static void Uninit();
	static void Update(const float& deltaTime);
	static void Draw();

	static void ChangeScene();

	static Scene* GetScene()
	{ 
		return m_Scene;
	}
	static LoadScene* GetLoadScene()
	{
		return m_LoadScene;
	}
	static bool& GetLoadFinish()
	{
		return m_LoadFinish;
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