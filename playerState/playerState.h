#pragma once
#include "playerState/playerStateMachine.h"

class Player;

class PlayerState
{
protected:
	Player* m_PlayerCache = nullptr;
	PlayerStateMachine* m_PlayerMachine = nullptr;

public:
	virtual ~PlayerState();
	PlayerState(PlayerStateMachine* machine);
	void ChangePlayerState(const PLAYER_STATE& playerState);

	virtual void Init() = 0;
	virtual void Unit() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual void ChangeState() = 0;		// ステートの切り替え処理
};