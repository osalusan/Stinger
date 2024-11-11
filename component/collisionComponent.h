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

struct OBB {
	XMVECTOR Center;	// OBB�̒��S
	XMVECTOR Axis[3];	// OBB�̊e���̕����i�P�ʃx�N�g���j
	XMVECTOR Size;		// �e�������̃T�C�Y

	// OBB�̕������擾
	const XMVECTOR& GetDirect(const int& index) const {
		return Axis[index];
	}

	// OBB�̃T�C�Y���擾
	const float GetSize(const int& index) const {
		switch (index) {
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
	bool m_Enable = false;		// �����蔻��̗L������
	XMFLOAT3 m_Pos = {};
	XMFLOAT3 m_Scale = {};

	XMFLOAT3 m_ModelCenter = {};
	XMFLOAT3 m_ModelScale = {};

	COLLISION_TAG m_CollisionTag = COLLISION_TAG::MAX;
	std::vector<GameObject*> m_HitGameObjectsCache = {};								// ���������I�u�W�F�N�g��ۑ�
	std::vector<GameObject*> m_GameObjectsCache[static_cast<int>(OBJECT::MAX)] = {};	// �I�u�W�F�N�g��ۑ�
	ObjectManager* m_ObjectManager = nullptr;

	bool CheckHitObject();
	// OBB���m�̓����蔻��
	bool HitOBB(const OBB& obb1, const OBB& obb2);
	float LenSegOnSeparateAxis(const XMVECTOR& Sep, const XMVECTOR& e1, const XMVECTOR& e2, const XMVECTOR& e3);
	void SetHitObject(GameObject* hitObj);
public:
	using Component::Component;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	bool CheckHitCollision(const COLLISION_TAG& tag);

	// �R���|�[�l���g���C���X�^���X�����鎞�Ɉꏏ��
	void SetCollisionTag(const COLLISION_TAG& tag)
	{
		m_CollisionTag = tag;
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