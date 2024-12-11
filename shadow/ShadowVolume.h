#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"

// 前方宣言
enum class TEXTURE;

class Polygon2D final :public GameObject
{
protected:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	TEXTURE m_Texture;						// 使用テクスチャ / コンストラクタで初期化


	VERTEX_3D m_Vertex[4] = {};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
public:
	Polygon2D() {};
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
	void SetPolygon(const XMFLOAT2& position, const XMFLOAT2& size);
};
