#pragma once
#include "character/character.h"
#include <unordered_map>

//前方宣言
class PlayerStateMachine;
class OtherPlayer :public Character 
{
private:
	PlayerStateMachine* m_PlayerStateMachine = nullptr;			// プレイヤーステート管理用

public:
	OtherPlayer();
	virtual ~OtherPlayer()override;

	virtual void Init()override;
	virtual void Uninit()override;

};