#include "playerStateParry.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"

void PlayerStateParry::Init()
{
	m_CurrentTime = 0.0f;
	if (m_ObjManagerCache != nullptr)
	{
		m_ObjManagerCache->SetSlowTime(0.9f);
		m_ObjManagerCache->SetSlowValue(0.0f);
	}
	if (m_ObjManagerCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		m_ObjManagerCache = scene->GetObjectManager();
	}
}

void PlayerStateParry::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateParry::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;
}

void PlayerStateParry::ChangeStateControl()
{
	// TODO :アニメーションの時間を設定
	if (m_CurrentTime >= 1.0f)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
		if (m_ObjManagerCache == nullptr) return;
		m_ObjManagerCache->SetSlowTime(0.0f);
	}
}
