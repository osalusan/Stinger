#pragma once
#include "playerState.h"
// 回避ステート
class PlayerStateRolling final :public PlayerState
{
private:
	XMFLOAT3 m_MoveVector = {};				// 回避入力時の移動方向を保存
	XMFLOAT3 m_NormalizeVelocity = {};
	float m_RollingSpeed = 0.0f;
	float m_SpeedAttenuateValue = 0.0f;		// 速度の減衰値
	float m_RotSpeed = 0.0f;
	bool m_RollingAccept = false;			// 回避成功
	float m_MinRollingAcceptTime = 0.0f;	// アニメーション中でも他のステートへ行けるようになる時間
	float m_MaxRollingAcceptTime = 0.0f;	// 回避の最大時間
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;

	bool CheckRollingAccept();
};