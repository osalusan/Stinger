#pragma once
#include "renderer/renderer.h"
#include <string>
#include <unordered_map>

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256] = { 0 };
	MATERIAL					Material;
	char						TextureName[256] = { 0 };
	ID3D11ShaderResourceView*	Texture = nullptr;
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex = 0;
	unsigned int	IndexNum = 0;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ
{
	VERTEX_3D*		VertexArray = nullptr;
	unsigned int	VertexNum = 0;

	unsigned int*	IndexArray = 0;
	unsigned int	IndexNum = 0;

	SUBSET*			SubsetArray = nullptr;
	unsigned int	SubsetNum = 0;

	XMFLOAT3     Origin = {}; // 原点を追加
	XMFLOAT3     Size = {};   // モデルのサイズ
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer = nullptr;
	ID3D11Buffer*	IndexBuffer = nullptr;

	SUBSET*			SubsetArray = nullptr;
	unsigned int	SubsetNum = 0;

	XMFLOAT3     Center = {}; // 原点を追加
	XMFLOAT3     Scale = {};   // モデルのサイズ
};
// OBJの読み込み処理を行うクラス
class ObjModelRenderer final
{	
private:
	MODEL* m_Model = {};
public:
	ObjModelRenderer(MODEL* model);
	void Draw();
	void DrawCollision();
	void Uninit();

	void SetModel(MODEL* model);

	MODEL* GetModel()
	{
		return m_Model;
	}
};