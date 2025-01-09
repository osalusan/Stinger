#include "manager/sceneManager.h"
#include "manager/fbxModelManager.h"
#include "manager/objModelManager.h"
#include "manager/textureManager.h"
#include "manager/inputManager.h"
#include "renderer/renderer.h"
#include "scene/gameScene.h"
#include "scene/titleScene.h"
#include "scene/loadScene.h"
#include "scene/debugScene.h"
#include "fade/fade.h"
#include "imgui/imguiWindow.h"
#include <thread>

Scene* SceneManager::m_Scene = nullptr;
Scene* SceneManager::m_NextScene = nullptr;
LoadScene* SceneManager::m_LoadScene = nullptr;
Fade* SceneManager::m_Fade = nullptr;
#if _DEBUG
ImguiWindow* SceneManager::m_ImguiWindow = nullptr;
#endif // _DEBUG
bool SceneManager::m_LoadFinish = true;
bool SceneManager::m_UseLoadScene = false;

void SceneManager::Init()
{
	Renderer::Init();
	InputManager::Init();

#if _DEBUG
	// RendererのInitの後に生成
	if (m_ImguiWindow == nullptr)
	{
		m_ImguiWindow = new ImguiWindow;
	}
#endif // _DEBUG


	// ロードシーンの作成 / GetSceneを前提に作られてるから、m_Sceneで一度作成しないといけない
	if (m_Scene == nullptr)
	{
		m_Scene = new LoadScene;
	}
	if (m_Scene != nullptr)
	{
		m_Scene->Init();
	}

	if (LoadScene* scene = dynamic_cast<LoadScene*>(m_Scene))
	{
		m_LoadScene = scene;
	}

	m_Scene = nullptr;

	if (m_Scene == nullptr)
	{
		// TODO :完成前にタイトルに変更 / デバッグ用
		m_Scene = new GameScene;
	}
	if (m_Scene != nullptr)
	{
		m_Scene->Init();
	}

	if (m_Fade == nullptr)
	{
		m_Fade = new Fade;
	}
	if (m_Fade != nullptr)
	{
		m_Fade->Init();
	}

#if _DEBUG
	// SceneのInitの後に
	if (m_ImguiWindow != nullptr)
	{
		m_ImguiWindow->GetRootNode(m_Scene);
	}
#endif // _DEBUG

	// 一番最後に
	FbxModelManager::Init();
	ObjModelManager::Init();
	TextureManager::Init();
}


void SceneManager::Uninit()
{	
	if (m_Fade != nullptr)
	{
		m_Fade->Uninit();
	}
	delete m_Fade;
	m_Fade = nullptr;

	if (m_LoadScene != nullptr)
	{
		m_LoadScene->Uninit();
	}
	delete m_LoadScene;
	m_LoadScene = nullptr;

	if (m_Scene != nullptr)
	{
		m_Scene->Uninit();
	}
	
	delete m_Scene;
	m_Scene = nullptr;

#if _DEBUG
	delete m_ImguiWindow;
	m_ImguiWindow = nullptr;
#endif // _DEBUG

	FbxModelManager::Uninit();
	ObjModelManager::Uninit();
	TextureManager::Uninit();
	Renderer::Uninit();
}

void SceneManager::Update(const float& deltaTime)
{
	InputManager::Update();

#if _DEBUG
	if (m_ImguiWindow != nullptr)
	{
		m_ImguiWindow->Update(deltaTime);
	}
#endif // _DEBUG

	if (m_LoadScene == nullptr) return;
	if (m_Scene == nullptr) return;
	if (m_Fade == nullptr) return;

	if (m_UseLoadScene)
	{
		if (m_LoadScene == nullptr) return;
		m_LoadScene->Update(deltaTime);
	}
	else
	{
		m_Scene->Update(deltaTime);
	}

	m_Fade->Update(deltaTime);

	// ロードシーンからそれぞれのシーンへ
	if (m_UseLoadScene)
	{
		if (!m_LoadScene->GetLoad())
		{
			if (m_Fade->GetFadeInComplete())
			{
				m_Fade->StartFadeOut();
			}

			if (m_Fade->GetFadeOutComplete())
			{
				m_Fade->StartFadeIn();
				m_UseLoadScene = false;
#if _DEBUG
				m_ImguiWindow->GetRootNode(m_Scene);
#endif // _DEBUG
			}
		}
	}

	// ロードシーンへの切り替え
	if (m_NextScene != nullptr)
	{
		if (!m_LoadFinish) return;
		// フェードインフラグリセット用
		m_Fade->GetFadeInComplete();

		m_Fade->StartFadeOut();
		m_LoadScene->Update(deltaTime);

		if (!m_Fade->GetFadeOutComplete()) return;


		// ロード開始
		m_LoadScene->LoadStart();
		m_Fade->StartFadeIn();
		m_LoadFinish = false;
		m_UseLoadScene = true;
#if _DEBUG
		m_ImguiWindow->ClearNode();
#endif // _DEBUG

		std::thread th(&ChangeScene); // スレッド
		th.detach();
	}
}

void SceneManager::Draw()
{
	Renderer::Begin();

	if (m_Scene != nullptr)
	{
		if (m_UseLoadScene)
		{
			if (m_LoadScene == nullptr) return;
			m_LoadScene->Draw();
		}
		else
		{
			m_Scene->Draw();
		}
	}

	if (m_Fade != nullptr)
	{
		m_Fade->Draw();
	}

#if _DEBUG
	if (m_ImguiWindow != nullptr)
	{
		m_ImguiWindow->Draw();
	}
#endif // _DEBUG

	Renderer::End();
}

void SceneManager::ChangeScene()
{
	if (m_NextScene != nullptr)
	{
		if (m_Scene != nullptr)
		{
			m_Scene->Uninit();
		}
		delete m_Scene;
		m_Scene = nullptr;

		m_Scene = m_NextScene;

		if (m_Scene != nullptr)
		{
			m_Scene->Init();
		}
	
		m_NextScene = nullptr;

		InputManager::Init();
		FbxModelManager::Init();
		ObjModelManager::Init();
		TextureManager::Init();	
	}

	// ロード終了
	m_LoadFinish = true;
}

