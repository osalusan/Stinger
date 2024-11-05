#include "gameObject.h"
#include "renderer/renderer.h"

GameObject::~GameObject()
{
	// シェーダーの削除
	if (m_VertexLayout != nullptr)
	{
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}
	if (m_VertexShader != nullptr)
	{
		m_VertexShader->Release();
		m_VertexLayout = nullptr;
	}
	if (m_PixelShader != nullptr)
	{
		m_PixelShader->Release();
		m_VertexLayout = nullptr;
	}	
}

void GameObject::Init()
{
	// コンストラクタでshaderを指定しなかった場合
	if (m_VertexShader == nullptr && m_VertexLayout == nullptr)
	{
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"cso\\unlitTextureVS.cso");
	}
	if (m_PixelShader == nullptr)
	{
		Renderer::CreatePixelShader(&m_PixelShader, "cso\\unlitTexturePS.cso");
	}
}

void GameObject::Draw()
{
	if (m_VertexLayout != nullptr && m_VertexShader != nullptr && m_PixelShader != nullptr)
	{
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
	}
	
	XMMATRIX world, scl, rot, trans;

	scl = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);
}

// --------------------- private ---------------------

void GameObject::GetForward(XMFLOAT3& forward)const
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMStoreFloat3(&forward, rotationMatrix.r[2]);
}

void GameObject::GetRight(XMFLOAT3& forward)const
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMStoreFloat3(&forward, rotationMatrix.r[0]);
}

void GameObject::LoadShader(const std::string& vsFileName, const std::string& psFileName)
{
	if (m_VertexShader == nullptr && m_VertexLayout == nullptr)
	{
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, vsFileName.c_str());
	}
	if (m_PixelShader == nullptr)
	{
		Renderer::CreatePixelShader(&m_PixelShader, psFileName.c_str());
	}
}
