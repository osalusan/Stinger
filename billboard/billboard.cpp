#include "billboard.h"
#include "camera/camera.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
#include "component/shaderComponent.h"

BillBoard::BillBoard(const XMFLOAT3& position, const XMFLOAT3& size, const TEXTURE& texture, const wchar_t* fileName, const float& nextAnimTime, const XMINT2& sprite, const int& maxTexture)
{
	// 初期化
	m_Texture = TEXTURE::MAX;

	m_Texture = texture;

	m_Position.x = position.x;
	m_Position.y = position.y;
	m_Scale.x = size.x;
	m_Scale.y = size.y;

	m_NextAnimTime = nextAnimTime;
	m_Sprite = sprite;

	if (maxTexture == 0)
	{
		m_MaxCount = m_Sprite.x * m_Sprite.y;
	}
	else
	{
		m_MaxCount = maxTexture;
	}

	TextureManager::ReservTexture(m_Texture, fileName);

	AddComponent<ShaderComponent>(this);
}
void BillBoard::Init()
{
	m_Vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f + m_Offset.y);

	m_Vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f + m_Offset.y);

	m_Vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	m_Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	m_Vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_Vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

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

void BillBoard::Uninit()
{
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Release();
	}
}

void BillBoard::Update(const float& deltaTime)
{
	if (m_CurrentTime >= m_NextAnimTime)
	{ 
		m_AnimCount++; 
		m_CurrentTime = 0.0f;
	}
	if (m_AnimCount >= m_MaxCount)
	{ 
		m_AnimCount = 0; 
	}
	m_CurrentTime += deltaTime;
}
void BillBoard::Draw()
{
	GameObject::Draw();

	if (m_Sprite.x != 0)
	{
		//テクスチャ座標算出(剰余斬)
		float x = m_AnimCount % m_Sprite.x * (1.0f / m_Sprite.x);
		float y = m_AnimCount / m_Sprite.x * (1.0f / m_Sprite.y);

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		m_Vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
		m_Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[0].TexCoord = XMFLOAT2(x, y);

		m_Vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
		m_Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[1].TexCoord = XMFLOAT2(x + (1.0f / m_Sprite.x), y);

		m_Vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		m_Vertex[2].Normal = XMFLOAT3(-1.0f, 0.0f, -1.0f);
		m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[2].TexCoord = XMFLOAT2(x, y + (1.0f / m_Sprite.y));

		m_Vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		m_Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertex[3].TexCoord = XMFLOAT2(x + (1.0f / m_Sprite.x), y + (1.0f / m_Sprite.y));

		memcpy(msr.pData, m_Vertex, sizeof(VERTEX_3D) * 4);
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	if (m_CameraCache == nullptr) return;
	XMMATRIX view = m_CameraCache->GetViewMatrix();

	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//ワールドマトリクス設定
	XMMATRIX world, scale, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

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

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

