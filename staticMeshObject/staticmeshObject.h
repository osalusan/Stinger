#pragma once
#include "object/gameObject.h"

// �O���錾
class ObjModelRenderer;
enum class STATICMESH_MODEL;
struct MODEL;

class StaticMeshObject :public GameObject
{
protected:
	STATICMESH_MODEL m_Model;						// ���f���{�� / �R���X�g���N�^�ŏ�����
	ObjModelRenderer* m_ModelRenderer = nullptr;

public:
	StaticMeshObject() = delete;
	StaticMeshObject(const STATICMESH_MODEL& model);
	virtual ~StaticMeshObject();
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	const MODEL* GetModel();
};