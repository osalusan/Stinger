#include "player.h"
#include "manager/fbxModelManager.h"

Player::Player() :Character(ANIMETION_MODEL::PLAYER)
{
	FbxModelManager::ReservModel(m_Model, "asset\\model\\Akai.fbx");
}

void Player::Init()
{
	GameObject::Init();
	m_Scale = { 0.03f,0.03f,0.03f };
}

void Player::Uninit()
{
	
}

// ------------------------------- private -------------------------------
void Player::MoveControl(const float& deltaTime)
{
}

void Player::CollisionControl()
{

}
