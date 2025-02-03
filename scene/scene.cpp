#include "scene.h"
#include "manager/objectManager.h"
#include "manager/particleManager.h"
#include "camera/camera.h"

// -------------------------- protected --------------------------
void Scene::CreateParticleManager()
{
	if (m_ParticleManager == nullptr)
	{
		m_ParticleManager = new ParticleManager;
	}
	if (m_ParticleManager != nullptr)
	{
		m_ParticleManager->Init();
	}
}

// -------------------------- public --------------------------
Scene::~Scene()
{
	delete m_ParticleManager;
	m_ParticleManager = nullptr;
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
	if (m_ParticleManager != nullptr)
	{
		m_ParticleManager->Uninit();
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
	// ƒJƒƒ‰‚ÌŒã‚ÉXV
	if (m_ParticleManager != nullptr)
	{
		m_ParticleManager->Update(deltaTime);
	}
}

void Scene::Draw()
{
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Draw();
	}

	if (m_ParticleManager != nullptr)
	{
		m_ParticleManager->Draw();
	}
}
