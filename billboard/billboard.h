#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"

// 前方宣言
enum class TEXTURE;
class Camera;

class BillBoard :public GameObject
{
protected:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	Camera* m_CameraCache = nullptr;
	TEXTURE m_Texture;						// 使用テクスチャ / コンストラクタで初期化

	VERTEX_3D m_Vertex[4] = {};

	// スプライトシートの画像枚数
	XMINT2 m_Sprite = {};
	float m_CurrentTime = 0;
	int m_AnimCount = 0;
	int m_MaxCount = 0;
	float m_NextAnimTime = 0.0f;
	bool m_Loop = false;
	int m_LoopStart = 0;
	int m_LoopEnd = 0;
	XMFLOAT2 m_Offset = { 0.0f,0.0f };
public:
	BillBoard() = delete;
	~BillBoard();
																																		// スプライトシートに余白がある場合は最大枚数を指定
	BillBoard(const XMFLOAT3& position, const XMFLOAT3& size, const TEXTURE& texture, const wchar_t* fileName,const float& nextAnimTime ,const XMINT2& sprite,const int& maxTexture = 0);

	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	void UseBillboard();
};