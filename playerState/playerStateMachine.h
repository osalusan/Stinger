#pragma once
#include "main/main.h"
#include <unordered_map>
#include <string>

enum class PLAYER_STATE
{
	NONE = 0,
	IDLE,
	JUMP,
	DAMAGE,
	HOLD_WEAPON,
	PARRY,
	RUN,
	DEAD,
	MAX
};

enum class MOVE_DIRECTION
{
	NONE = 0,
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	MAX
};
// 前方宣言
class PlayerState;
class Player;
class Camera;

class PlayerStateMachine final
{
private:
	Player* m_PlayerCache = nullptr;
	Camera* m_CameraCache = nullptr;

	PlayerState* m_CurrentPlayerState = nullptr;
	PLAYER_STATE m_CurrentState = PLAYER_STATE::NONE;
	std::unordered_map<PLAYER_STATE, PlayerState*> m_PlayerStatePool = {};	

	// 移動方向記録用
	MOVE_DIRECTION m_RandL = MOVE_DIRECTION::NONE;	// 右か左のどちらかに移動している
	MOVE_DIRECTION m_FandB = MOVE_DIRECTION::NONE;	// 前と後ろのどちらかに移動している

	XMFLOAT3 m_Velocity = {};
	XMFLOAT3 m_Rotation = {};

	bool m_IsGround = false;						// 地面に触れているか
	bool m_IsJamp = false;							// ジャンプしたか
	bool m_IsHold = false;							// 構えているか
	bool m_IsHitAttacked = false;					// 攻撃を受けたか
	bool m_IsInvincible = false;					// 無敵状態かどうか

	std::string m_NextAnimationName = "";			// アニメーションの名前

public:
	PlayerStateMachine() = delete;
	PlayerStateMachine(Player* player);
	~PlayerStateMachine();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);

	void SetPlayerState(const PLAYER_STATE& state);
	void InitVelocity();

	XMFLOAT3 GetCameraForward()const;
	XMFLOAT3 GetCameraRight()const;
	void HitGround();					// 地面に当たった

	const Player* GetPlayerCache()const
	{
		return m_PlayerCache;
	}
	const PLAYER_STATE& GetCurrentState()const
	{
		return m_CurrentState;
	}

	const XMFLOAT3& GetVelocity()const
	{
		return m_Velocity;
	}
	const XMFLOAT3& GetRotation()const
	{
		return m_Rotation;
	}

	const MOVE_DIRECTION& GetMoveRandL()const
	{
		return m_RandL;
	}
	const MOVE_DIRECTION& GetMoveFandB()const
	{
		return m_FandB;
	}
	const bool& GetIsJump()const
	{
		return m_IsJamp;
	}
	const bool& GetIsGround()const
	{
		return m_IsGround;
	}
	const bool& GetIsHold()const
	{
		return m_IsHold;
	}
	const bool& GetIsHitAttacked()const
	{
		return m_IsHitAttacked;
	}
	const bool& GetIsInvincible()const
	{
		return m_IsInvincible;
	}
	const std::string& GetAnimation()const
	{
		return m_NextAnimationName;
	}

	void SetIsHitDamage(const bool& hit)
	{
		m_IsHitAttacked = hit;
	}
	void SetAnimation(const std::string& anime)
	{
		m_NextAnimationName = anime;
	}
	void SetVelocityX(const float& x)
	{
		m_Velocity.x = x;
	}
	void SetVelocityZ(const float& z)
	{
		m_Velocity.z = z;
	}
	void SetVelocityY(const float& y)
	{
		m_Velocity.y = y;
	}
	void SetRotationY(const float& y)
	{
		m_Rotation.y = y;
	}
};