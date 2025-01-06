#pragma once
#include "playerState/playerStateMachine.h"

class Player;

class PlayerState
{
protected:
	PlayerStateMachine* m_PlayerMachine = nullptr;

	std::string m_AnimeName = {};
	bool m_LoadAnimation = false;

	void LoadAnimation(const std::string& fileName, const std::string& animationName);
public:
	virtual ~PlayerState();
	PlayerState(PlayerStateMachine* machine);
	void ChangePlayerState(const PLAYER_STATE& playerState);

	virtual void Init() = 0;
	virtual void Unit() = 0;
	virtual void Update(const float& deltaTime);
	virtual void ChangeStateControl() = 0;		// ステートの切り替え処理
};