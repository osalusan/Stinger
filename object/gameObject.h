#pragma once
#include "main/main.h"
#include <string>

class GameObject {
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };
	
	bool m_Enable = false;		// �L���A����

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	void GetForward(XMFLOAT3& forward)const;
	void GetRight(XMFLOAT3& forward)const;
	// �R���X�g���N�^�ŌĂ�
	void LoadShader(const std::string& vsFileName, const std::string& psFileName);
public:
	virtual ~GameObject();
	virtual void Init();
	virtual void Uninit() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual void Draw();

	const bool& GetEnable()const
	{
		return m_Enable;
	}
	const XMFLOAT3& GetPos()const
	{
		return m_Position;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
};