#pragma once
#include "character/character.h"

class Camera;
class PlayerStateMachine;

class Player final:public Character {
private:

	Camera* m_CameraCache = nullptr;
	PlayerStateMachine* m_PlayerStateMachine = nullptr;

	bool m_ChengeAnimation = false;				// �A�j���[�V���������ς����邩�ǂ���
	int m_AnimationFrame = 0;					// �A�j���[�V�����̃t���[��
	std::string m_AnimationName = "Idle";		// �A�j���[�V�����̖��O
	std::string m_NextAnimationName = "Idle";	// �u�����h�p�A���̃A�j���[�V�����̖��O
	float m_BlendRatio = 1.0f;					// �A�j���[�V�����u�����h�̐��l
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