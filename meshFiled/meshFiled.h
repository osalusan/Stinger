#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"
#define FILED_MAX (51)

enum class TEXTURE;
// メッシュフィールド
class MeshFiled :public GameObject
{
protected:
	TEXTURE m_Texture;						// 使用テクスチャ / コンストラクタで初期化
	TEXTURE m_Normal;						// 使用法線 / コンストラクタで初期化

	// 頂点管理
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	VERTEX_3D m_Vertex[FILED_MAX][FILED_MAX] = {};

	LIGHT m_Light;
	float m_FiledHeight[FILED_MAX][FILED_MAX] ={};

	// CSVの数字をフィールドのY座標に反映させる
	void FiledDataLoadCSV(const std::string& filePath);
public:
	MeshFiled() = delete;
	// ファイルはDDS形式のみ読み込める
	MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture,const wchar_t* textureName, std::string filePath = "");
	MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture, const wchar_t* textureName,const TEXTURE& normal, const wchar_t* normalName, std::string filePath = "");
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	// XZ座標からY座標の高さが分かる
	float GetHeight(const XMFLOAT3& position);
};