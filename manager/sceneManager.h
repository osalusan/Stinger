#pragma once
#include "main/main.h"

// 前方宣言
class Scene;
class LoadScene;
class Fade;
class ImguiWindow;
// ゲーム内すべてのSceneを管理するクラス
class SceneManager final
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static LoadScene* m_LoadScene;
	static Fade* m_Fade;
#if _DEBUG
	static ImguiWindow* m_ImguiWindow;
#endif // _DEBUG
	static bool m_LoadFinish;
	static bool m_UseLoadScene;

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
	static bool GetLoadFinish()
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