#pragma once
#include "main/main.h"

class ObjectManager;
class ParticleManager;

class Scene 
{
protected:
	ObjectManager* m_ObjectManager = nullptr;
	ParticleManager* m_ParticleManager = nullptr;
	XMFLOAT3 m_WorldWall = {};

	void CreateParticleManager(); // オブジェクトの作成後に作成
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
	ParticleManager* GetParticleManager()
	{
		return m_ParticleManager;
	}
	const XMFLOAT3& GetWorldWall()const
	{
		return m_WorldWall;
	}
};