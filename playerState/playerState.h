#pragma once
#include "playerState/playerStateMachine.h"

class Player;

class PlayerState
{
protected:
	PlayerStateMachine* m_PlayerMachine = nullptr;

	std::string m_AnimName = {};
	bool m_LoadAnimation = false;
	float m_CurrentTime = 0.0f;
	float m_MaxAnimTime = 0.0f;

	void LoadAnimation(const std::string& fileName, const std::string& animationName);
	// �J�����̌����ɉ�]
	void RotToCameraDirection(const float& deltaTime);
public:
	virtual ~PlayerState();
	PlayerState(PlayerStateMachine* machine);
	void ChangePlayerState(const PLAYER_STATE& playerState);

	virtual void Init() = 0;
	virtual void Unit() = 0;
	virtual void Update(const float& deltaTime);
	virtual void ChangeStateControl() = 0;		// �X�e�[�g�̐؂�ւ�����
};