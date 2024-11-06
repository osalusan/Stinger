#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"

enum class PIVOT
{
	NONE = 0,
	CENTER,
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	MAX
};

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
	Polygon2D() = delete;
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName);
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};
