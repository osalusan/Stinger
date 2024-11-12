#pragma once
#include "main/main.h"
#include <unordered_map>

enum class PLAYER_STATE
{
	NONE = 0,
	IDLE,
	JUMP,
	DAMAGE,
	RUN,
	DEAD,
	MAX
};

class PlayerState;

class PlayerStateMachine final
{
private:
	PlayerState* m_CurrentPlayerState = nullptr;
	std::unordered_map<PLAYER_STATE, PlayerState*> m_PlayerStatePool = {};	

	XMFLOAT3 m_Velocity = {};

	bool m_IsGround = false;					// ínñ Ç…êGÇÍÇƒÇ¢ÇÈÇ©
	bool m_IsJamp = false;						// ÉWÉÉÉìÉvÇµÇΩÇ©

public:
	~PlayerStateMachine();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void SetPlayerState(const PLAYER_STATE& state);

};