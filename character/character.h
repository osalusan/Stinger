#pragma once
#include "object/gameObject.h"
#include "component/collisionComponent.h"

// 前方宣言
enum class ANIMETION_MODEL;

class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};
	XMFLOAT3 m_RecordPosition = {};					// 過去座標
	ANIMETION_MODEL m_Model;						// モデル本体 / コンストラクタで初期化

	// 重力
	bool m_EnableGravity = false;
	float m_GravityValue = 0.0f;

	int m_Health = 0;								// 体力
	int m_Damage = 0;								// 与えるダメージ
	bool m_IsDead = false;							// 死んでいる

	float m_AnimationTime = 0.0f;					// アニメーションのフレーム
	std::string m_AnimationName = "Idle";			// アニメーションの名前
	std::string m_NextAnimationName = "Idle";		// ブレンド用、次のアニメーションの名前
	float m_BlendRatio = 1.0f;						// アニメーションブレンドの数値

	virtual void MoveControl(const float& deltaTime) = 0;
	virtual void CollisionControl() = 0;
	virtual void CustomCollisionInfo() = 0;

	void TakeDamage(const int& atk);
	void ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path);
	
public:
	Character();
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override;
	
	void ChangeAnimation(const std::string& anime);
};