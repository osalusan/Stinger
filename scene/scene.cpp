#include "scene.h"
#include "manager/objectManager.h"
#include "camera/camera.h"

// -------------------------- public --------------------------
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
}

void Scene::Draw()
{
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Draw();
	}
}
