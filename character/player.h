#pragma once
#include "character/character.h"

enum class PLAYER_STATE {
	NONE = 0,
	IDEL,
	WALK,
	ATTACK,
	DOWN,
	MAX
};

class Camera;

class Player :public Character {
private:

	Camera* m_CameraCache = nullptr;
	bool m_ChengeAnimation = false;// アニメーションが今変えられるかどうか
	int m_AnimationFrame = 0;	  // アニメーションのフレーム
	std::string m_AnimationName = "Idle";// アニメーションの名前
	std::string m_NextAnimationName = "Idle";// ブレンド用、次のアニメーションの名前
	float m_BlendRatio = 1.0f;// アニメーションブレンドの数値
	bool m_UseAttack = false;
	
	virtual void MoveControl(const float& deltaTime)override;
	virtual void CollisionControl();
public:
	Player();
	virtual void Init()override;
	virtual void Uninit()override;
};