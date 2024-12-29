#include "main/main.h"
#include "objModelRenderer.h"

ObjModelRenderer::ObjModelRenderer(MODEL* model)
{
	m_Model = model;
}

void ObjModelRenderer::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Model->VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_Model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
	{
		// マテリアル設定
		Renderer::SetMaterial(m_Model->SubsetArray[i].Material.Material);

		// テクスチャ設定
		if (m_Model->SubsetArray[i].Material.Texture)
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Model->SubsetArray[i].Material.Texture);
		}

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
	}
}

void ObjModelRenderer::DrawCollision()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Model->VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_Model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
	{
		// マテリアル設定
		Renderer::SetMaterial(m_Model->SubsetArray[i].Material.Material);

		// テクスチャ設定
		if (m_Model->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Model->SubsetArray[i].Material.Texture);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
	}
}

void ObjModelRenderer::Uninit()
{
	if (m_Model->VertexBuffer == nullptr || m_Model->IndexBuffer == nullptr)return;

	m_Model->VertexBuffer->Release();
	m_Model->IndexBuffer->Release();

	for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
	{
		if (m_Model->SubsetArray[i].Material.Texture)
			m_Model->SubsetArray[i].Material.Texture->Release();
	}

	delete[] m_Model->SubsetArray;
}

void ObjModelRenderer::SetModel(MODEL* model)
{
	if (model == nullptr) return;

	m_Model = model;
}

