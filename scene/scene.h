#pragma once
#include "main/main.h"

class ObjectManager;
// シーンの既定クラス
class Scene 
{
protected:
	ObjectManager* m_ObjectManager = nullptr;		// 全てのオブジェクトを管理しているクラス
	XMFLOAT3 m_WorldWall = {};						// 最大移動範囲の格納用

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