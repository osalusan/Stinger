#pragma once
#include "main/main.h"
#include <unordered_map>
#include <string>

enum class PLAYER_STATE
{
	NONE = 0,
	IDLE,
	HITDAMAGE,
	ROLLING,
	ATTACK_PARRY,
	ATTACK_NORMAL,
	ATTACK_EXTR,
	RUN,
	DEAD,
	MAX
};

// 移動方向の記録用
enum class MOVE_DIRECTION
{
	NONE = 0,
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	MAX
};

//ゲストから受け取る用の構造体
struct OtherPlayerData
{
	int playerID = 0;
	PLAYER_STATE playerState = PLAYER_STATE::MAX;
	XMFLOAT3 pos = {};
	float hp = 0.0f;
};

#define MAX_PLAYER (2)

// 前方宣言
class PlayerState;
class Player;
class Camera;
class PlayerStateParryAttack;
class PlayerStateRolling;
class PlayerStateNormalAttack;
class PlayerStateExtrAttack;
class PlayerStateHitDamage;
class Polygon2D;
class BossEnemy;
// プレイヤーのステートを管理するクラス
class PlayerStateMachine final
{
private:
	Player* m_PlayerCache = nullptr;						// ステートの数値をプレイヤーに伝える
	Camera* m_CameraCache = nullptr;						// カメラのベクトル取得用
	Polygon2D* m_ParryGageCache = nullptr;					// パリィ成功時のゲージ操作用
	BossEnemy* m_BossCache = nullptr;						// ダメージ通知用

	PlayerState* m_CurrentPlayerState = nullptr;			// 現在のステート
	PlayerStateParryAttack* m_ParryCache = nullptr;			// パリィ判定用
	PlayerStateNormalAttack* m_NormalAttackCache = nullptr;	// 攻撃判定用
	PlayerStateExtrAttack* m_ExtrAttackCache = nullptr;		// 攻撃判定用

	PlayerStateRolling* m_RollingCache = nullptr;			// 回避中は体力が減らない処理の為
	PLAYER_STATE m_CurrentState = PLAYER_STATE::NONE;
	std::unordered_map<PLAYER_STATE, PlayerState*> m_PlayerStatePool = {};	

	// 移動方向記録用
	MOVE_DIRECTION m_RandL = MOVE_DIRECTION::NONE;	// 右か左のどちらかに移動している
	MOVE_DIRECTION m_FandB = MOVE_DIRECTION::NONE;	// 前と後ろのどちらかに移動している

	XMFLOAT3 m_Velocity = {};
	XMFLOAT3 m_Rotation = {};

	float m_AnimeBlendTimeValue = 0.0f;				// アニメーションのブレンド速度
	int m_ParryCount = 0;							// パリィカウント

	bool m_IsGround = false;						// 地面に触れているか
	bool m_IsJamp = false;							// ジャンプしたか
	bool m_IsParryAttackButton = false;				// パリィ攻撃をしたか
	bool m_IsRollingButton = false;					// 回転したか
	bool m_IsNormalAttackButton = false;			// 通常攻撃をしたか
	bool m_IsHitAttacked = false;					// 攻撃を受けたか
	bool m_IsInvincible = false;					// 無敵状態かどうか
	bool m_IsExtrAttack = false;					// エクストラ攻撃が発動可能か
	bool m_IsParry = false;							// パリィしているか

	std::string m_NextAnimationName = "";			// アニメーションの名前

	// 入力情報の初期化
	void InputReset();
public:
	PlayerStateMachine() = delete;
	PlayerStateMachine(Player* player);
	~PlayerStateMachine();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);

	void SetPlayerState(const PLAYER_STATE& state);
	void InitVelocity();
	void SetAnimationSpeedValue(const float& value);	// アニメーションの速度変更 / ステート変更時にリセット

	bool CheckParry();									// パリィ中か確認
	bool CheckRolling();								// 回避中か確認
	bool CheckAttack();									// 攻撃中歌確認
	XMFLOAT3 GetCameraForward()const;
	XMFLOAT3 GetCameraRight()const;
	void HitGround();									// 地面に当たった

	void UsedExtrAttack();								// エクストラ攻撃を使用した後にデータをリセット

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
	const bool& GetIsParryAttackButton()const
	{
		return m_IsParryAttackButton;
	}
	const bool& GetIsNormalAttackButton()const
	{
		return m_IsNormalAttackButton;
	}
	const bool& GetIsRollingButton()const
	{
		return m_IsRollingButton;
	}
	const bool& GetIsInvincible()const
	{
		return m_IsInvincible;
	}
	const std::string& GetAnimation()const
	{
		return m_NextAnimationName;
	}
	const bool& GetIsHitAttack()const
	{
		return m_IsHitAttacked;
	}	
	const bool& GetIsExtrAttack()const
	{
		return m_IsExtrAttack;
	}

	void SetIsHitAttack(const bool& hit)
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
	void SetAnimeBlendTimeValue(const float& value)
	{
		m_AnimeBlendTimeValue = value;
	}
};