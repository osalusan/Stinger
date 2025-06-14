#include "otherplayer.h"
#include "manager/fbxModelManager.h"
#include "playerState/playerStateMachine.h"

OtherPlayer::OtherPlayer()
{
	ReservModel(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\paladinJNordstrom.fbx");
}

OtherPlayer::~OtherPlayer()
{

}

void OtherPlayer::Init()
{
	if (m_PlayerStateMachine == nullptr)
	{
		m_PlayerStateMachine = new PlayerStateMachine(this);
	}
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Init();
	}
}

void OtherPlayer::Uninit()
{
}
