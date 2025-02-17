#pragma once
#include "main/main.h"

class ObjectManager;

class Scene 
{
protected:
	ObjectManager* m_ObjectManager = nullptr;
	XMFLOAT3 m_WorldWall = {};

public:
	virtual ~Scene();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	ObjectManager* GetObjectManager()
	{
		return m_ObjectManager;
	}
	const XMFLOAT3& GetWorldWall()const
	{
		return m_WorldWall;
	}
};