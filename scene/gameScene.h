#pragma once
#include "scene/scene.h"

// ‘O•ûéŒ¾
class ObjectManager;
class PlayerCamera;

class GameScene final :public Scene 
{
private:
	ObjectManager* m_ObjectManager = nullptr;
	PlayerCamera* m_PlayerCamera = nullptr;
public:
	virtual ~GameScene();
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	ObjectManager* GetObjectManager()
	{
		return m_ObjectManager;
	}
	PlayerCamera* GetPlayerCamera()
	{
		return m_PlayerCamera;
	}
};