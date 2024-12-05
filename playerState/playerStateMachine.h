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
class Camera;

class PlayerStateMachine final
{
private:
	Camera* m_CameraCache = nullptr;
	PlayerState* m_CurrentPlayerState = nullptr;
	std::unordered_map<PLAYER_STATE, PlayerState*> m_PlayerStatePool = {};	

	// 移動方向記録用
	MOVE_DIRECTION m_RandL = MOVE_DIRECTION::NONE;	// 右か左のどちらかに移動している
	MOVE_DIRECTION m_FandB = MOVE_DIRECTION::NONE;	// 前と後ろのどちらかに移動している

	XMFLOAT3 m_Velocity = {};

	bool m_IsGround = false;						// 地面に触れているか
	bool m_IsJamp = false;							// ジャンプしたか

	std::string m_NextAnimationName = "";			// アニメーションの名前

public:
	~PlayerStateMachine();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);

	void SetPlayerState(const PLAYER_STATE& state);
	void InitVelocity();

	XMFLOAT3 GetCameraForward()const;
	XMFLOAT3 GetCameraRight()const;
	void HitGround();					// 地面に当たった

	const XMFLOAT3& GetVelocity()const
	{
		return m_Velocity;
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
	const std::string& GetAnimation()const
	{
		return m_NextAnimationName;
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
};