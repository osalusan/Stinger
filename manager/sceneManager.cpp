#include "manager/sceneManager.h"
#include "scene/scene.h"
#include "renderer/renderer.h"

Scene* SceneManager::m_Scene;
Scene* SceneManager::m_NextScene;

void SceneManager::Init()
{
	Renderer::Init();
	if (m_Scene == nullptr)
	{
		m_Scene = new Scene;
	}
	if (m_Scene != nullptr)
	{
		m_Scene->Init();
	}
}


void SceneManager::Uninit()
{	
	m_Scene->Uninit();
	delete m_Scene;

	Renderer::Uninit();
}

void SceneManager::Update()
{

}

void SceneManager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();

	if (m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}
		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
		m_Scene->Update();

	}
}

