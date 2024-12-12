#include "gameObject.h"
#include "renderer/renderer.h"
#include "component/boxCollisionComponent.h"

// --------------------- private ---------------------

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

void GameObject::AddBoxCollisionComponent(const COLLISION_TAG& tag)
{
	if (m_BoxCollision == nullptr)
	{
		m_BoxCollision = new BoxCollisionComponent(this);
	}
	if (m_BoxCollision != nullptr)
	{
		m_BoxCollision->Init();
		m_BoxCollision->SetCollisionTag(tag);
	}
}

void GameObject::UpdateBoxCollisionInfo()
{
	if (m_BoxCollision != nullptr)
	{
		m_BoxCollision->SetBoxCollisionInfo(m_ColliPosition, m_ColliScale, m_ModelCenter, m_ModelScale, GetColliRotationMatrix());
	}
}

// --------------------- public ---------------------

GameObject::~GameObject()
{
	delete m_BoxCollision;
	m_BoxCollision = nullptr;

	// �V�F�[�_�[�̍폜
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
	// �R���X�g���N�^��shader���w�肵�Ȃ������ꍇ
	if (m_VertexShader == nullptr && m_VertexLayout == nullptr)
	{
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"cso\\unlitTextureVS.cso");
	}
	if (m_PixelShader == nullptr)
	{
		Renderer::CreatePixelShader(&m_PixelShader, "cso\\unlitTexturePS.cso");
	}
}

void GameObject::Uninit()
{
	if (m_BoxCollision != nullptr)
	{
		m_BoxCollision->Uninit();
	}
}

void GameObject::Update(const float& deltaTime)
{
	if (m_BoxCollision != nullptr)
	{
		m_BoxCollision->Update();
	}
}

void GameObject::Draw()
{
	// ��ԍŏ��ɕ`��
#if _DEBUG
	UpdateBoxCollisionInfo();
	if (m_BoxCollision != nullptr)
	{
		m_BoxCollision->Draw();
	}
#endif // _DEBUG

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


