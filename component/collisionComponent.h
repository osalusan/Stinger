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
	XMVECTOR Center;	// OBB�̒��S
	XMVECTOR Axis[3];	// OBB�̊e���̕����i�P�ʃx�N�g���j
	XMVECTOR Size;		// �e�������̃T�C�Y

	// OBB�̃T�C�Y���擾
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

// �O���錾
class ObjectManager;

#if _DEBUG
enum class STATICMESH_MODEL;
class ObjModelRenderer;
#endif // _DEBUG

class CollisionComponent :public Component
{
protected:
	bool m_Enable = false;		// �����蔻��̗L������

	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Scale = {};
	XMFLOAT3 m_ModelCenter = {};
	XMFLOAT3 m_ModelScale = {};
	XMMATRIX m_RotationMatrix = {};
	
#if _DEBUG
	// �����蔻��`��p
	STATICMESH_MODEL m_Model;						// Init()�ŏ�����
	ObjModelRenderer* m_ModelRenderer = nullptr;
#endif // _DEBUG



	// �����o���p
	XMVECTOR m_MtvAxis = {};		// MTV�̎�
	float m_MinOverlap = 0.0f;		// �ŏ��I�[�o�[���b�v��

	COLLISION_TAG m_CollisionTag = COLLISION_TAG::MAX;
	std::vector<GameObject*> m_HitGameObjectsCache = {};								// ���������I�u�W�F�N�g��ۑ�
	std::vector<GameObject*> m_GameObjectsCache[static_cast<int>(OBJECT::MAX)] = {};	// �I�u�W�F�N�g��ۑ�

	virtual bool CheckHitObject();

	bool HitOBB(const OBB& obb1, const OBB& obb2);
	float LenSegOnSeparateAxis(const XMVECTOR& Sep, const OBB& obb);
	void SetHitObject(GameObject* hitObj);
public:
	using Component::Component;
	virtual ~CollisionComponent()override;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	void SetBoxCollisionInfo(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& modelCenterPos, const XMFLOAT3& modelScale,const XMMATRIX& rotateMatrix);

	bool CheckHitCollision(const COLLISION_TAG& tag);

	// �R���|�[�l���g���C���X�^���X�����鎞�Ɉꏏ��
	void SetCollisionTag(const COLLISION_TAG& tag)
	{
		m_CollisionTag = tag;
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

	const COLLISION_TAG& GetCollisionTag()
	{
		return m_CollisionTag;
	}

	// ���������I�u�W�F�N�g����擾
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

	// ���������I�u�W�F�N�g�����ׂĎ擾
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