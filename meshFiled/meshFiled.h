#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"
#define FILED_MAX (31)

enum class TEXTURE;
class MeshFiled :public GameObject
{
private:
	TEXTURE m_Texture;						// 使用テクスチャ / コンストラクタで初期化
	TEXTURE m_Normal;						// 使用法線 / コンストラクタで初期化

	// 頂点管理
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	VERTEX_3D m_Vertex[FILED_MAX][FILED_MAX] = {};

	LIGHT m_Light;
public:
	MeshFiled();
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	float GetHeight(XMFLOAT3 position);
};