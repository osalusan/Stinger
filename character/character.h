#pragma once
#include "object/gameObject.h"

#define DEFAULT_BLEND_VALUE (8.0f)

// 前方宣言
enum class ANIMETION_MODEL;

class MeshFiled;
class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};
	XMFLOAT3 m_RecordPosition = {};					// 過去座標
	ANIMETION_MODEL m_Model;						// モデル本体 / コンストラクタで初期化

	MeshFiled* m_MeshFiled = nullptr;

	XMFLOAT3 m_WorldWall = {};						// 移動限界
	// 重力
	bool m_EnableGravity = false;
	float m_GravityValue = 0.0f;

	// 共通のパラメータ
	float m_Health = 0;								// 体力
	float m_MaxHealth = 0;							// 最大体力
	float m_RotSpeed = 0.0f;						// 回転速度
	float m_Attack = 0.0f;							// 攻撃力
	float m_MoveSpeed = 0.0f;						// 移動速度

	// 演出終了フラグ
	bool m_FinishPerformance = false;

	// アニメーション用
	float m_AnimationTime = 0.0f;					// アニメーションのフレーム
	float m_NextAnimationTime = 0.0f;				// 次のアニメーションのフレーム
	std::string m_AnimationName = "";				// アニメーションの名前
	std::string m_NextAnimationName = "";			// ブレンド用、次のアニメーションの名前
	float m_BlendRatio = 0.0f;						// アニメーションブレンドの数値
	float m_BlendTimeValue = 0.0f;					// どのくらいブレンドを早く行うか
	float m_MaxAnimeTime = 0.0f;					// 現在のアニメーションの最大時間

	virtual void MoveControl(const float& deltaTime) = 0;
	virtual void CollisionControl() = 0;
	virtual void CustomCollisionInfo() = 0;
	virtual void AnimationControl() = 0;

	virtual void ParameterControl(const float& deltaTime);

	void ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path);
	void CheckWorldWallPos();		// ワールドの範囲外に出たら範囲内に戻す
public:
	Character();
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override;

	virtual void TakeDamage(const float& atk) = 0;

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
};