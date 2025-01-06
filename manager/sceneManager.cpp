#include "manager/sceneManager.h"
#include "manager/fbxModelManager.h"
#include "manager/objModelManager.h"
#include "manager/textureManager.h"
#include "manager/inputManager.h"
#include "renderer/renderer.h"
#include "scene/gameScene.h"
#include "scene/titleScene.h"
#include "scene/loadScene.h"
#include "fade/fade.h"
#include <thread>

Scene* SceneManager::m_Scene = nullptr;
Scene* SceneManager::m_NextScene = nullptr;
LoadScene* SceneManager::m_LoadScene = nullptr;
Fade* SceneManager::m_Fade = nullptr;
bool SceneManager::m_LoadFinish = true;

void SceneManager::Init()
{
	Renderer::Init();
	InputManager::Init();

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

	FbxModelManager::Uninit();
	ObjModelManager::Uninit();
	TextureManager::Uninit();
	Renderer::Uninit();
}

void SceneManager::Update(const float& deltaTime)
{
	InputManager::Update();

	if (m_Scene != nullptr)
	{
		if (m_LoadScene->GetLoad())
		{
			if (m_LoadScene == nullptr) return;
			m_LoadScene->Update(deltaTime);
		}
		else
		{
			m_Scene->Update(deltaTime);
		}	
	}

	if (m_Fade == nullptr) return;
	m_Fade->Update(deltaTime);


	if (m_LoadScene->GetLoad() && m_LoadFinish)
	{
		m_Fade->GetFadeInComplete();

		//m_Fade->StartFadeOut();
		//if (m_LoadScene == nullptr) return;

		//if (!m_Fade->GetFadeOutComplete()) return;

		//m_Fade->StartFadeIn();
	}
}

void SceneManager::Draw()
{
	Renderer::Begin();

	if (m_Scene != nullptr)
	{
		if (m_LoadScene->GetLoad())
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

	Renderer::End();

	if (m_NextScene != nullptr)
	{
		if (!m_LoadFinish) return;

		if (m_Fade != nullptr)
		{
			m_Fade->StartFadeOut();
			if (m_LoadScene == nullptr) return;

			if (!m_Fade->GetFadeOutComplete()) return;
		}

		// ロード開始
		m_LoadScene->LoadStart();
		m_LoadFinish = false;

		if (m_Fade != nullptr)
		{
			m_Fade->StartFadeIn();
		}

		std::thread th(&ChangeScene); // スレッド
		th.detach();
	}
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
	
		InputManager::Init();
		FbxModelManager::Init();
		ObjModelManager::Init();
		TextureManager::Init();

		m_NextScene = nullptr;
	}

	// ロード終了
	m_LoadFinish = true;
}

