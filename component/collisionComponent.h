#pragma once
#include "component.h"
#include "main/main.h"
#include "manager/objectManager.h"

enum class COLLISION_TAG
{
	PLAYER,
	ENEMY_BOSS,
	OBJECT,
	MAX
};

struct OBB 
{
	XMVECTOR Center;	// OBBの中心
	XMVECTOR Axis[3];	// OBBの各軸の方向（単位ベクトル）
	XMVECTOR Size;		// 各軸方向のサイズ

	// OBBのサイズを取得
	const float GetSize(const int& index) const 
	{
		switch (index) 
		{
		case 0:
			return XMVectorGetX(Size);
		case 1:
			return XMVectorGetY(Size);
		case 2:
			return XMVectorGetZ(Size);
		default:
			return 0.0f;
		}
	}
};

class ObjectManager;

class CollisionComponent :public Component
{
protected:
	bool m_Enable = false;		// 当たり判定の有効無効

	XMFLOAT3 m_Pos = {};
	XMFLOAT3 m_Scale = {};
	XMFLOAT3 m_ModelCenter = {};
	XMFLOAT3 m_ModelScale = {};
	XMMATRIX m_RotationMatrix = {};

	// 押し出し用
	XMVECTOR m_MtvAxis = {};		// MTVの軸
	float m_MinOverlap = 0.0f;		// 最小オーバーラップ量

	COLLISION_TAG m_CollisionTag = COLLISION_TAG::MAX;
	std::vector<GameObject*> m_HitGameObjectsCache = {};								// 当たったオブジェクトを保存
	std::vector<GameObject*> m_GameObjectsCache[static_cast<int>(OBJECT::MAX)] = {};	// オブジェクトを保存

	virtual bool CheckHitObject();

	bool HitOBB(const OBB& obb1, const OBB& obb2);
	float LenSegOnSeparateAxis(const XMVECTOR& Sep, const OBB& obb);
	void SetHitObject(GameObject* hitObj);
public:
	using Component::Component;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	void SetBoxCollisionInfo(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& modelCenterPos, const XMFLOAT3& modelScale,const XMMATRIX& rotateMatrix);

	bool CheckHitCollision(const COLLISION_TAG& tag);

	// コンポーネントをインスタンス化する時に一緒に
	void SetCollisionTag(const COLLISION_TAG& tag)
	{
		m_CollisionTag = tag;
	}

	const XMFLOAT3& GetPos()const
	{
		return m_Pos;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}

	const XMFLOAT3& GetModelCenter()const
	{
		return m_ModelCenter;
	}
	const XMFLOAT3& GetModelScale()const
	{
		return m_ModelScale;
	}
	const XMMATRIX& GetRotationMatrix()const
	{
		return m_RotationMatrix;
	}

	const COLLISION_TAG& GetCollisionTag()
	{
		return m_CollisionTag;
	}

	// 当たったオブジェクトを一つ取得
	template <typename T>
	T* GetHitGameObject()
	{
		for (GameObject* object : m_HitGameObjectsCache)
		{
			if (object == nullptr) continue;

			if (T* hitObject = dynamic_cast<T*>(object))
			{
				return hitObject;
			}
		}

		return nullptr;
	}

	// 当たったオブジェクトをすべて取得
	template <typename T>
	bool GetHitGameObjects(std::vector<T*>& objectList)
	{
		for (GameObject* object : m_HitGameObjectsCache)
		{
			if (object == nullptr) continue;

			if (T* hitObject = dynamic_cast<T*>(object))
			{
				objectList.emplace_back(hitObject);
			}
		}
		return !objectList.empty();
	}
};