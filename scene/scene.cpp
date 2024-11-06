#include "scene.h"
#include "manager/objectManager.h"
#include "camera/camera.h"

Scene::~Scene()
{
	delete m_ObjectManager;
	m_ObjectManager = nullptr;
}

void Scene::Init()
{
	if (m_ObjectManager == nullptr)
	{
		m_ObjectManager = new ObjectManager;
	}
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Init();
	}
}

void Scene::Uninit()
{
	if (m_Camera != nullptr)
	{
		m_Camera->Uninit();
	}

	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Uninit();
	}
}

void Scene::Update(const float& deltaTime)
{
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Update(deltaTime);
	}

	// プレイヤーの後に更新
	if (m_Camera != nullptr)
	{
		m_Camera->Update();
	}
}

void Scene::Draw()
{
	// 一番最初に描画
	if (m_Camera != nullptr)
	{
		m_Camera->Draw();
	}

	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Draw();
	}
}
