#pragma once
#include "playerState.h"

class ObjectManager;
class BossEnemy;
class PlayerStateNormalAttack final :public PlayerState
{
private:
	bool m_AttackAccept = false;
	ObjectManager* m_ObjManagerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;

	std::string m_AnimName1 = {};
	std::string m_AnimName2 = {};
	std::string m_AnimName3 = {};

	float m_MaxAnimTime1 = 0.0f;
	float m_MaxAnimTime2 = 0.0f;
	float m_MaxAnimTime3 = 0.0f;

	// 連続攻撃時に待機モーションをキャンセルして別の行動へ移行できる
	float m_Attack1CancleValue = 0.55f;
	float m_Attack2CancleValue = 0.55f;
	float m_Attack3CancleValue = 0.72f;

	bool m_AttackCancel = false;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};