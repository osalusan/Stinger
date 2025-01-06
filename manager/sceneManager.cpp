#include "manager/sceneManager.h"
#include "manager/fbxModelManager.h"
#include "manager/objModelManager.h"
#include "manager/textureManager.h"
#include "manager/inputManager.h"
#include "renderer/renderer.h"
#include "scene/gameScene.h"
#include "scene/titleScene.h"
#include "scene/loadScene.h"
#include <thread>

Scene* SceneManager::m_Scene = nullptr;
Scene* SceneManager::m_NextScene = nullptr;
Scene* SceneManager::m_LoadScene = nullptr;
bool SceneManager::m_LoadFinish = true;

void SceneManager::Init()
{
	Renderer::Init();
	InputManager::Init();

	if (m_Scene == nullptr)
	{
		// TODO :�����O�Ƀ^�C�g���ɕύX / �f�o�b�O�p
		m_Scene = new GameScene;
	}
	if (m_Scene != nullptr)
	{
		m_Scene->Init();
	}

	if (m_LoadScene == nullptr)
	{
		m_LoadScene = new LoadScene;
	}
	if (m_LoadScene != nullptr)
	{
		m_LoadScene->Init();
	}

	// ��ԍŌ��
	FbxModelManager::Init();
	ObjModelManager::Init();
	TextureManager::Init();
}


void SceneManager::Uninit()
{	
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
		if (!m_LoadFinish)
		{
			if (m_LoadScene == nullptr) return;

			m_LoadScene->Update(deltaTime);
		}
		else
		{
			m_Scene->Update(deltaTime);
		}	
	}
}

void SceneManager::Draw()
{
	Renderer::Begin();

	if (m_Scene != nullptr)
	{
		if (!m_LoadFinish)
		{
			if (m_LoadScene == nullptr) return;

			m_LoadScene->Draw();
		}
		else
		{
			m_Scene->Draw();
		}	
	}

	Renderer::End();

	if (m_NextScene != nullptr)
	{
		if (m_LoadFinish)
		{
			std::thread th(&ChangeScene); // �X���b�h
			th.detach();
		}
	}
}

void SceneManager::ChangeScene()
{
	if (m_NextScene != nullptr)
	{
		// ���[�h�J�n
		m_LoadFinish = false;

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

		// ���[�h�I��
		//m_LoadFinish = true;
	}
}

