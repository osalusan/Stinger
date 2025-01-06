#pragma once
#include "character/character.h"

class Camera;
class PlayerStateMachine;
class BoxCollisionComponent;

class Player final:public Character {
private:

	BoxCollisionComponent* m_BoxCollCache = nullptr;

	PlayerStateMachine* m_PlayerStateMachine = nullptr;

	bool m_ChengeAnimation = false;				// アニメーションが今変えられるかどうか
	bool m_UseAttack = false;

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;
	void PlayerDataLoadCSV(const std::string& filePath);
public:
	Player();
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;

	const XMFLOAT3& GetVelocity()const
	{
		return m_Velocity;
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