#pragma once
#include "character/character.h"

class Camera;
class PlayerStateMachine;

class Player final:public Character {
private:

	Camera* m_CameraCache = nullptr;
	PlayerStateMachine* m_PlayerStateMachine = nullptr;

	bool m_ChengeAnimation = false;				// アニメーションが今変えられるかどうか
	int m_AnimationFrame = 0;					// アニメーションのフレーム
	std::string m_AnimationName = "Idle";		// アニメーションの名前
	std::string m_NextAnimationName = "Idle";	// ブレンド用、次のアニメーションの名前
	float m_BlendRatio = 1.0f;					// アニメーションブレンドの数値
	bool m_UseAttack = false;

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
public:
	Player();
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;
};