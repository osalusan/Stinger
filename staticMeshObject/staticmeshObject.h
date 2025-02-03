#pragma once
#include "object/gameObject.h"

// �O���錾
enum class STATICMESH_MODEL;
class ObjModelRenderer;

class StaticMeshObject :public GameObject
{
protected:
	STATICMESH_MODEL m_Model;						// ���f���{�� / �R���X�g���N�^�ŏ�����
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	XMFLOAT4 m_Color = {0.0f,1.0f,0.0f,1.0f};
	XMMATRIX m_RotationMatrix = {};

	bool m_Visible = true;		// �����Ȃ��Ȃ��Ă������蔻��͎c��

	virtual void MoveControl(const float& deltaTime) = 0;
public:
	StaticMeshObject() = delete;
	StaticMeshObject(const STATICMESH_MODEL& model);
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	const ObjModelRenderer* GetModel()const;

	const STATICMESH_MODEL& GetStaticModel()const
	{
		return m_Model;
	}
};