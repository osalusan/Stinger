#pragma once
#include "character/character.h"

enum class PLAYER_STATE {
	IDEL = 0,
	WALK,
	ATTACK,
	DOWN,
	MAX
};

class Player :public Character {
private:

	bool _chengeAnimation = false;// �A�j���[�V���������ς����邩�ǂ���
	int _animationFrame = 0;	  // �A�j���[�V�����̃t���[��
	std::string _animationName = "Idle";// �A�j���[�V�����̖��O
	std::string _nextanimationName = "Idle";// �u�����h�p�A���̃A�j���[�V�����̖��O
	float _blendRatio = 1.0f;// �A�j���[�V�����u�����h�̐��l
	bool _useAttack = false;
	
	virtual void MoveControl(const float& deltaTime)override;
	virtual void CollisionControl();
public:
	Player();
	virtual void Init()override;
	virtual void Uninit()override;
};