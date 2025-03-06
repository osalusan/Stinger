#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"
#define FILED_MAX (31)

enum class TEXTURE;
class MeshFiled :public GameObject
{
protected:
	TEXTURE m_Texture;						// �g�p�e�N�X�`�� / �R���X�g���N�^�ŏ�����
	TEXTURE m_Normal;						// �g�p�@�� / �R���X�g���N�^�ŏ�����

	// ���_�Ǘ�
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	VERTEX_3D m_Vertex[FILED_MAX][FILED_MAX] = {};

	LIGHT m_Light;
public:
	MeshFiled() = delete;
	MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture,const wchar_t* textureName);
	MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture, const wchar_t* textureName,const TEXTURE& normal, const wchar_t* normalName);
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	float GetHeight(XMFLOAT3 position);
};