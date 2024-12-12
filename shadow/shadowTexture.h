#pragma once
#include "renderer/renderer.h"
#include"object/gameObject.h"

class ShadowTexture :public GameObject
{
protected:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = {};
	ID3D11PixelShader* m_PixelShader = {};
	ID3D11InputLayout* m_VertexLayout = {};

	VERTEX_3D vertex[4];

	int _drowType = 0;

public:
	void Init();
	void Unit();
	void Update();
	void Draw();

	virtual void Load(const wchar_t* FileName);
	//•`‰æ‚Ìƒpƒ^[ƒ“‚ğŒˆ‚ß‚é
};