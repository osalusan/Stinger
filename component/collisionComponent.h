#pragma once
#include "component.h"
#include "main/main.h"
#include "manager/objectManager.h"
#include <string>

enum class COLLISION_TAG
{
	PLAYER,
	ENEMY_BOSS,
	OBJECT,
	EQUIPMENT_OBJECT,
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

#if _DEBUG
enum class STATICMESH_MODEL;
#endif // _DEBUG

class CollisionComponent :public Component
{
private:
	// ボーンにコリジョンをつける用
	void UseBoneMatrix();

protected:
	std::string m_CollisionName = "";
	bool m_Enable = false;		// 当たり判定の有効無効

	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Scale = {};
	XMFLOAT3 m_ModelCenter = {};
	XMFLOAT3 m_ModelScale = {1.0f,1.0f,1.0f};  // 必要ない時の為に初期値１
	XMMATRIX m_RotationMatrix = {};
	XMMATRIX m_BoneMatrix = {};

#if _DEBUG
	// 当たり判定描画用
	STATICMESH_MODEL m_Model;						// Init()で初期化

	// 描画関連
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
#endif // _DEBUG

	// 押し出し用
	XMVECTOR m_MtvAxis = {};		// MTVの軸
	float m_MinOverlap = 0.0f;		// 最小オーバーラップ量

	COLLISION_TAG m_CollisionTag = COLLISION_TAG::MAX;
	std::vector<GameObject*> m_HitGameObjectsCache = {};								// 当たったオブジェクトを保存
	std::vector<GameObject*> m_GameObjectsCache[static_cast<int>(OBJECT::MAX)] = {};	// オブジェクトを保存

	virtual bool CheckHitObject(const OBJECT& object);
	virtual bool CheckHitObject(const COLLISION_TAG& tag);
	virtual bool CheckHitAllObject();

	bool HitOBB(const OBB& obb1, const OBB& obb2);
	float LenSegOnSeparateAxis(const XMVECTOR& Sep, const OBB& obb);
	void SetHitObject(GameObject* hitObj);
public:
	using Component::Component;
	CollisionComponent(GameObject* gameObject,const COLLISION_TAG& tag);
	CollisionComponent(GameObject* gameObject,const COLLISION_TAG& tag,const std::string& name);
	virtual ~CollisionComponent()override;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	// 毎フレーム値を入れるように
	void SetCollisionInfo(const XMFLOAT3& pos,const XMFLOAT3& scale, const XMFLOAT3& modelCenterPos, const XMFLOAT3& modelScale,const XMMATRIX& rotateMatrix); // 大きさを変えたい時
	void SetCollisionInfo(const XMFLOAT3& pos, const XMFLOAT3& scl, const XMFLOAT3& modelCenterPos, const XMFLOAT3& modelScale, const XMMATRIX& rotateMatrix, const XMMATRIX& worldMatrix);	   // 本体に付属させたい時
	void SetScale(const XMFLOAT3& scl)
	{
		m_Scale = scl;
	}
	// コンポーネントをインスタンス化する時に一緒に
	void SetCollisionTag(const COLLISION_TAG& tag)
	{
		m_CollisionTag = tag;
	}
	void SetPos(const XMFLOAT3& pos)
	{
		m_Position = pos;
	}

	const std::string& GetName()
	{
		return m_CollisionName;
	}
	const XMFLOAT3& GetPos()const
	{
		return m_Position;
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
	const COLLISION_TAG& GetCollisionTag()const
	{
		return m_CollisionTag;
	}
	const XMMATRIX& GetBoneMatrix()const
	{
		return m_BoneMatrix;
	}

	const GameObject* GetGameObject()
	{
		return m_GameObject;
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