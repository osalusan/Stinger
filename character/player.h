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
	bool m_ChengeAnimation = false;// �A�j���[�V���������ς����邩�ǂ���
	int m_AnimationFrame = 0;	  // �A�j���[�V�����̃t���[��
	std::string m_AnimationName = "Idle";// �A�j���[�V�����̖��O
	std::string m_NextAnimationName = "Idle";// �u�����h�p�A���̃A�j���[�V�����̖��O
	float m_BlendRatio = 1.0f;// �A�j���[�V�����u�����h�̐��l
	bool m_UseAttack = false;
	
	virtual void MoveControl(const float& deltaTime)override;
	virtual void CollisionControl();
public:
	Player();
	virtual void Init()override;
	virtual void Uninit()override;
};