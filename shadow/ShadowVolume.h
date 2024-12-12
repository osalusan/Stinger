#pragma once
#include "object/gameObject.h"

// �O���錾
class ObjModelRenderer;
enum class STATICMESH_MODEL;
enum class ANIMETION_MODEL;
struct MODEL;

class ShadowVolume final:public GameObject
{
protected:
	STATICMESH_MODEL m_StaticModel;						// ���f���{�� / �R���X�g���N�^�ŏ�����
	ANIMETION_MODEL m_AnimeModel;						// ���f���{�� / �R���X�g���N�^�ŏ�����
	ObjModelRenderer* m_ModelRenderer = nullptr;

public:
	ShadowVolume() = delete;
	ShadowVolume(const STATICMESH_MODEL& model);
	ShadowVolume(const ANIMETION_MODEL& model);
	virtual ~ShadowVolume()override;
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};