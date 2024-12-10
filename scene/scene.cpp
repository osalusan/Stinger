#include "scene.h"
#include "manager/objectManager.h"
#include "manager/particleManager.h"
#include "camera/camera.h"

// -------------------------- protected --------------------------
void Scene::CreateParticleManager()
{

}

// -------------------------- public --------------------------
Scene::~Scene()
{
	delete m_ParticleManager;
	m_ParticleManager = nullptr;
	delete m_Camera;
	m_Camera = nullptr;
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
	// カメラの後に更新
	if (m_ParticleManager != nullptr)
	{
		m_ParticleManager->Update(deltaTime);
	}
}

void Scene::Draw()
{
	// 一番最初に描画
	if (m_Camera != nullptr)
	{
		m_Camera->Draw();
	}
	// オブジェクトより前に描画
	if (m_ParticleManager != nullptr)
	{
		m_ParticleManager->Draw();
	}
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Draw();
	}
}
