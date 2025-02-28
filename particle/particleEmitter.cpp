#include "renderer/renderer.h"
#include "particleEmitter.h"
#include "camera/playerCamera.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
#include "component/shaderComponent.h"

// ---------------------------------- public ----------------------------------
ParticleEmiter::ParticleEmiter()
{
	m_Texture = TEXTURE::MAX;
}

ParticleEmiter::ParticleEmiter(const XMFLOAT3& pos)
	:ParticleEmiter()
{
	m_Position = pos;
}

ParticleEmiter::ParticleEmiter(const XMFLOAT3& pos, const bool& disable)
	:ParticleEmiter(pos)
{
	m_Enable = disable;
}

ParticleEmiter::ParticleEmiter(const bool& disable)
	:ParticleEmiter()
{
	m_Enable = disable;
}

ParticleEmiter::~ParticleEmiter()
{
	m_CameraCache = nullptr;
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Release();
	}
}

void ParticleEmiter::Init()
{
	GameObject::Init();
	ReservTexture();

	if (m_Texture == TEXTURE::MAX)
	{
		m_Texture = TEXTURE::PARTICLE_ORIGIN;
		TextureManager::ReservTexture(m_Texture, L"asset\\texture\\particleOrigin.png");
	}

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		m_Particle[i].color = { 1.0f,1.0f,1.0f,1.0f };
	}
	m_Mix = true;

	if (m_CameraCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		m_CameraCache = objManager->GetCamera();
	}


	// 最後のコンポーネントとして追加
	AddComponent<ShaderComponent>(this);
}

void ParticleEmiter::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);
	// カメラより前に作られた時用
	if (m_CameraCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		m_CameraCache = objManager->GetCamera();
	}

	if (m_Enable)
	{
		// パーティクルの作成が先
		CreateParticleEffect(deltaTime);
	}

	// パーティクルを作成した後に配置
	UpdateParticleEffect(deltaTime);

	if (!m_ReservEnable && m_ParticleLifeTime != 0.0f)
	{
		if (m_CurrentParticleLifeTime >= m_ParticleLifeTime)
		{
			m_Enable = false;
			m_CurrentParticleLifeTime = 0.0f;
		}
		m_CurrentParticleLifeTime += deltaTime;
	}
}

void ParticleEmiter::Draw()
{
	GameObject::Draw();
	if (m_CameraCache == nullptr) return;
	XMMATRIX view = m_CameraCache->GetViewMatrix();

	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//頂点バッファの設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(m_Texture);
	if (texture == nullptr) return;

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	//プリミティブポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Zバッファ無効
	Renderer::SetDepthEnable(false);
	//加算合成を有効
	if (m_Mix)
	{
		Renderer::SetBlendAddEnable(true);
	}


	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].enable)
		{
			//マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = m_Particle[i].color;
			material.TextureEnable = true;
			Renderer::SetMaterial(material);

			//ワールドマトリクス設定
			XMMATRIX world, scale, trans;
			scale = XMMatrixScaling(m_Particle[i].scale.x, m_Particle[i].scale.y, m_Particle[i].scale.z);
			trans = XMMatrixTranslation(m_Particle[i].position.x, m_Particle[i].position.y, m_Particle[i].position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			//ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}
	//Zバッファ有効
	Renderer::SetDepthEnable(true);

	//加算合成を無効
	if (m_Mix)
	{
		Renderer::SetBlendAddEnable(false);
	}
}

void ParticleEmiter::End()
{
	m_ReservEnable = false;
}

void ParticleEmiter::Start()
{
	m_Enable = true;
	m_ReservEnable = true;
}
