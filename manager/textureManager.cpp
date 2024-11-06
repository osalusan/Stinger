#include "manager/textureManager.h"
#include <string>

std::unordered_map<TEXTURE, ID3D11ShaderResourceView*> TextureManager::m_LoadTexturePool = {};
std::unordered_map<TEXTURE, wchar_t*> TextureManager::m_ReservTexturePool = {};

// ----------------------------- private -----------------------------
void TextureManager::LoadTexture( const TEXTURE& textureName ,wchar_t* fileName)
{
	TexMetadata metadata = {};
	ScratchImage image = {};

	if (m_LoadTexturePool.count(textureName) > 0)
	{
		return;
	}

	ID3D11ShaderResourceView* texture = nullptr;

	LoadFromWICFile(fileName, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);

	if (texture == nullptr) return;
	m_LoadTexturePool.emplace(textureName, texture);
}

// ----------------------------- public -----------------------------
void TextureManager::Init()
{
	for (const std::pair<const TEXTURE, wchar_t*>& reservModel : m_ReservTexturePool)
	{
		LoadTexture(reservModel.first,reservModel.second);
	}

	m_ReservTexturePool.clear();
}

void TextureManager::Uninit()
{
	for (std::pair<const TEXTURE, ID3D11ShaderResourceView*>& loadModel : m_LoadTexturePool)
	{
		if (loadModel.second != nullptr)
		{
			loadModel.second->Release();
		}
		delete loadModel.second;
		loadModel.second = nullptr;
	}
}

void TextureManager::ReservTexture(const TEXTURE& texture, wchar_t* fileName)
{
	m_ReservTexturePool.emplace(texture, fileName);
}

ID3D11ShaderResourceView*& TextureManager::GetTexture(const TEXTURE& texture)
{
	return m_LoadTexturePool[texture];
}
