#include "playerStateHoldWeapon.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
void PlayerStateHoldWeapon::Init()
{
	m_CurrentTime = 0.0f;
	if (m_ObjManagerCache != nullptr)
	{
		// TODO :変更予定 / パリィ演出確認用
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

void PlayerStateHoldWeapon::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateHoldWeapon::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;
}

void PlayerStateHoldWeapon::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	// 優先順位順

	// TODO :変更予定 / パリィ演出確認用
	if (!m_PlayerMachine->GetIsHold())
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
		if (m_ObjManagerCache == nullptr) return;
		m_ObjManagerCache->SetSlowTime(0.0f);
	}
}
