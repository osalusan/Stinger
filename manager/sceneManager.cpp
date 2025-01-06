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

	// ���[�h�V�[���̍쐬 / GetScene��O��ɍ���Ă邩��Am_Scene�ň�x�쐬���Ȃ��Ƃ����Ȃ�
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
		// TODO :�����O�Ƀ^�C�g���ɕύX / �f�o�b�O�p
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

	// ��ԍŌ��
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

		// ���[�h�J�n
		m_LoadScene->LoadStart();
		m_LoadFinish = false;

		if (m_Fade != nullptr)
		{
			m_Fade->StartFadeIn();
		}

		std::thread th(&ChangeScene); // �X���b�h
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

	// ���[�h�I��
	m_LoadFinish = true;
}

