#pragma once
#include "character/character.h"
#include <unordered_map>

//�O���錾
class PlayerStateMachine;
class OtherPlayer :public Character 
{
private:
	PlayerStateMachine* m_PlayerStateMachine = nullptr;			// �v���C���[�X�e�[�g�Ǘ��p

public:
	OtherPlayer();
	virtual ~OtherPlayer()override;

	virtual void Init()override;
	virtual void Uninit()override;

};