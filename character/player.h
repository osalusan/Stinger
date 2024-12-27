#pragma once
#include "character/character.h"

class Camera;
class PlayerStateMachine;
class BoxCollisionComponent;

class Player final:public Character {
private:

	Camera* m_CameraCache = nullptr;
	BoxCollisionComponent* m_BoxCollCache = nullptr;

	PlayerStateMachine* m_PlayerStateMachine = nullptr;

	bool m_ChengeAnimation = false;				// アニメーションが今変えられるかどうか
	bool m_UseAttack = false;

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;
public:
	Player();
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;
};