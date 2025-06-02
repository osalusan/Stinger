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
class Camera;
// テクスチャの既定クラス
class Polygon2D final :public GameObject
{
protected:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	TEXTURE m_Texture;							// 使用テクスチャ / コンストラクタで初期化
	PIVOT m_PivotPoint = PIVOT::NONE;			// 画像の原点

	VERTEX_3D m_Vertex[4] = {};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	bool m_UseStencil = false;					// 陰用に実装
	bool m_UseUI = false;						// UIとして使う / 画像のサイズやUVを変更できるように
	Camera* m_CameraCache = nullptr;
public:
	Polygon2D() = delete;
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName);
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName,const bool& ui);
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const bool& useStencil, const wchar_t* fileName);
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const bool& useStencil, const wchar_t* fileName,const bool& enable);

	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
	// 座標、大きさ、UV値を変更
	void SetPolygon(const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT2& uvValue = { 1.0f,1.0f });
	// 画像の大きさを変えずにUVだけ変更 / m_UseUIがtrueじゃないと変更不可
	void ChangeUVScaling(const XMFLOAT2& uvValue);		

	void SetAlfa(const float& alfa)
	{
		m_Color.w = alfa;
	}
	const TEXTURE& GetTexture()const
	{
		return m_Texture;
	}
};
