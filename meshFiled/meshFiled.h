#pragma once
#include "renderer/renderer.h"
#define FILED_MAX (31)

enum class TEXTURE;
class MeshFiled
{
private:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };

	TEXTURE m_Texture;						// 使用テクスチャ / コンストラクタで初期化
	TEXTURE m_Normal;						// 使用法線 / コンストラクタで初期化

	// 描画関連
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	// 頂点管理
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	VERTEX_3D m_Vertex[FILED_MAX][FILED_MAX] = {};

	LIGHT m_Light;
public:
	MeshFiled();
	~MeshFiled();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(XMFLOAT3 position);
};