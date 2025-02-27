#pragma once
#include "playerState.h"

// 前方宣言
class ObjectManager;
class BossEnemy;
class PlayerCamera;
class ExtrSlashEffect;
class ExtrSwordBarst;

class PlayerStateExtrAttack final :public PlayerState
{
private:
	ObjectManager* m_ObjManagerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;
	PlayerCamera* m_CameraCache = nullptr;
	ExtrSlashEffect* m_ExtrSlashEfCache = nullptr;
	ExtrSwordBarst* m_ExtrSwordBarstCache = nullptr;

	// 制御用パラメータ
	bool m_AttackAccept = false;
	bool m_AttackEnable = false;
	bool m_ChageSound = false;
	float m_CurrentMoveTime = 0.0f;
	XMFLOAT3 m_MoveSpeedVec = {};

	// アニメーション制御用パラメータ
	float m_AnimSpeedValue = 0.0f;
	bool m_StopAnim = false;
	float m_CurrentStopAnimTime = 0.0f;

	// CSVからの読み込み
	float m_DamageValue = 0.0f;
	float m_CutInTimeMax = 0.0f;
	float m_CutInTimeMin = 0.0f;
	float m_MoveTimeMax = 0.0f;
	float m_StartStopAnimTimeValue = 0.0f;
	float m_StopAnimTimeValue = 0.0f;
	float m_SlowAnimSpeedValue = 0.0f;

public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
	bool CheckAttackAccept();

	const bool& GetAttackEnable()const
	{
		return m_AttackEnable;
	}
};