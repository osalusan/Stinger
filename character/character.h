#pragma once
#include "object/gameObject.h"

#define DEFAULT_BLEND_VALUE (8.0f)

// 前方宣言
enum class ANIMETION_MODEL;
class MeshFiled;
// プレイヤーやエネミーの基底クラス
class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};						// 速度 / x,zは毎フレーム0にする
	XMFLOAT3 m_RecordPosition = {};					// 過去座標
	ANIMETION_MODEL m_Model;						// モデル本体 / コンストラクタで初期化

	MeshFiled* m_MeshFiledCache = nullptr;			// 地面の判定取得用のポインタ

	XMFLOAT3 m_WorldWall = {};						// 移動限界
	// 重力のパラメータ
	bool m_EnableGravity = false;					// 重力の有効無効
	float m_GravityValue = 0.0f;					// キャラクターの重さ
	float m_CurrentGravityValue = 0.0f;				// 時間の経過で増加していく重力値 / 上のgravityValueと掛ける

	// キャラクター共通のパラメータ
	float m_Health = 0;								// 体力
	float m_MaxHealth = 0;							// 最大体力
	float m_RotSpeed = 0.0f;						// 回転速度
	float m_Attack = 0.0f;							// 攻撃力
	float m_MoveSpeed = 0.0f;						// 移動速度
	bool m_IsDead = false;							// 生死

	bool m_IsFinishPerformance = false;				// 演出終了フラグ

	// アニメーション用
	float m_AnimationTime = 0.0f;					// アニメーションのフレーム
	float m_NextAnimationTime = 0.0f;				// 次のアニメーションのフレーム
	std::string m_AnimationName = "";				// アニメーションの名前
	std::string m_NextAnimationName = "";			// ブレンド用、次のアニメーションの名前
	float m_BlendRatio = 0.0f;						// アニメーションブレンドの数値
	float m_BlendTimeValue = 0.0f;					// どのくらいブレンドを早く行うか
	float m_MaxAnimeTime = 0.0f;					// 現在のアニメーションの最大時間
	float m_AnimSpeedValue = 1.0f;					// アニメーションの速度変更用

	virtual void MoveControl(const float& deltaTime) = 0;	// 自身の移動処理を行う
	virtual void CollisionControl() = 0;					// 自身の他のオブジェクトの判定処理を行う
	virtual void CustomCollisionInfo() = 0;					// 自身のコリジョンの設定を行う
	virtual void AnimationControl() = 0;					// 自身のアニメーションの設定を行う

	virtual void ParameterControl(const float& deltaTime);
	// FBXのモデルを予約 / コンストラクタや初期化で呼ぶ
	void ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path);
	// ワールドの範囲外に出たら範囲内に戻す
	void CheckWorldWallPos();		
public:
	Character();
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override;

	virtual void TakeDamage(const float& atk) = 0;		// ダメージを受けたときに、それぞれの子クラスでフラグなどを変更する為
	void ResetGravity();								// 重力値をリセットしたい時に
	void IsDead();										// プレイヤーステートや生死判定タスクで取得する為

	void ChangeAnimation(const std::string& anime)
	{
		m_NextAnimationName = anime;
	}

	void SetAnimeTime(const float& time)
	{
		m_AnimationTime = time;
	}
	void SetNextAnimationTime(const float& time)
	{
		m_NextAnimationTime = time;
	}
	void SetHealth(const float& health)
	{
		m_Health = health;
	}
	void SetBlendTimeValue(const float& value)
	{
		m_BlendTimeValue = value;
	}
	void SetAnimationSpeedValue(const float& value)
	{
		m_AnimSpeedValue = value;
	}
	const ANIMETION_MODEL& GetAnimeModel()const
	{
		return m_Model;
	}
	const std::string& GetAnimeName()const
	{
		return m_AnimationName;
	}
	const std::string& GetNextAnimeName()const
	{
		return m_NextAnimationName;
	}
	const float& GetAnimeTime()const
	{
		return m_AnimationTime;
	}
	const float& GetBlendRatio()const
	{
		return m_BlendRatio;
	}
	const float& GetMaxAnimeTime()const
	{
		return m_MaxAnimeTime;
	}
	
	// パラメータのGet
	const float& GetHealth()const
	{
		return m_Health;
	}	
	const float& GetMaxHealth()const
	{
		return m_MaxHealth;
	}
	const float& GetAttack()const
	{
		return m_Attack;
	}
	const float& GetMoveSpeed()const
	{
		return m_MoveSpeed;
	}
	const float& GetRotSpeed()const
	{
		return m_RotSpeed;
	}
	const bool& GetIsDead()const
	{
		return m_IsDead;
	}
};