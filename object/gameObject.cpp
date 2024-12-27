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
	BoxCollisionComponent* boxCollision = new BoxCollisionComponent(this);
	if (boxCollision == nullptr) return;

	boxCollision->Init();
	boxCollision->SetCollisionTag(tag);

	CollisionData* colliData = new CollisionData;
	if (colliData == nullptr) return;

	colliData->BoxCollisions = boxCollision;

	m_BoxCollisions.emplace_back(colliData);
}

void GameObject::UpdateBoxCollisionInfo()
{
	// 何も追加しない時の初期値
	for (CollisionData* colliData : m_BoxCollisions)
	{
		if (colliData == nullptr) continue;
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->SetBoxCollisionInfo(colliData->ColliPosition, colliData->ColliScale, m_ModelCenter, m_ModelScale, colliData->GetColliRotationMatrix());
	}
}

// --------------------- public ---------------------

GameObject::~GameObject()
{
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;

		delete boxCollision;
		boxCollision = nullptr;
	}

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

void GameObject::Uninit()
{
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->Uninit();
	}
}

void GameObject::Update(const float& deltaTime)
{
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->Update();
	}
}

void GameObject::Draw()
{
	// 一番最初に描画
#if _DEBUG
	UpdateBoxCollisionInfo();
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->Draw();
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


