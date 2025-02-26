#pragma once
#include "playerState.h"

class ObjectManager;
class BossEnemy;
class PlayerStateNormalAttack final :public PlayerState
{
private:
	BossEnemy* m_BossCache = nullptr;

	int m_AttackComboNumber = 0;
	float m_AttackDamage = 0.0f;
	float m_AnimSpeedValue = 0.0f;	// アニメーション速度

	std::string m_AnimName1 = {};
	std::string m_AnimName2 = {};
	std::string m_AnimName3 = {};

	float m_MaxAnimTime1 = 0.0f;
	float m_MaxAnimTime2 = 0.0f;
	float m_MaxAnimTime3 = 0.0f;

	// 連続攻撃時に待機モーションをキャンセルして別の行動へ移行できる
	float m_AttackCancleValue1 = 0.0f;
	float m_AttackCancleValue2 = 0.0f;
	float m_AttackCancleValue3 = 0.0f;

	// ダメージ倍率
	float m_DamageValue1 = 0.0f;
	float m_DamageValue2 = 0.0f;
	float m_DamageValue3 = 0.0f;

	// ダメージ発生開始時間割合
	float m_AttackEnableTimeValue1 = 0.0f;
	float m_AttackEnableTimeValue2 = 0.0f;
	float m_AttackEnableTimeValue3 = 0.0f;

	bool m_AttackCancel = false;
	bool m_UseAttack = false;
	bool m_AttackAccept = false;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
	bool CheckAttackAccept();

	const bool& GetAttackAccept()
	{
		return m_UseAttack;
	}
};