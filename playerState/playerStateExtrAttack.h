#pragma once
#include "playerState.h"

// 前方宣言
class ObjectManager;
class BossEnemy;
class PlayerCamera;
class ExtrSlashEffect;
class ExtrSwordBarst;
// エクストラ攻撃のステート
class PlayerStateExtrAttack final :public PlayerState
{
private:
	BossEnemy* m_BossCache = nullptr;					// ダメージ通知用
	PlayerCamera* m_PlayerCameraCache = nullptr;		// カメラの位置調整用
	ExtrSlashEffect* m_ExtrSlashEfCache = nullptr;		// ビルボードのエフェクト
	ExtrSwordBarst* m_ExtrSwordBarstCache = nullptr;	// 剣のオーラのパーティクル

	// 制御用パラメータ
	bool m_AttackAccept = false;
	bool m_AttackEnable = false;
	bool m_ChageSound = false;
	float m_CurrentMoveTime = 0.0f;
	XMFLOAT3 m_MoveSpeedVec = {};

	// アニメーション制御用パラメータ
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