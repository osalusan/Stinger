#pragma once
#include "renderer/renderer.h"
#include <unordered_map>

enum class TEXTURE
{
	TITLE = 0,
	PARTICLE_ORIGIN,
	GROUND_GREEN_TEX,
	GROUND_GREEN_NOR,
	LOADING,
	WHITE,
	BLACK,
	SHADOW,
	HPBAR_PLAYER,
	HPBAR_BOSS,
	HPBAR_BG_BOSS,
	HPBAR_FRAME_BOSS,
	EFFECT_LIGHTNING_BALL,
	EFFECT_LIGHTNING_FALL,
	PUSHSPACEKEY,
	GAME_CLEAR,
	GAME_OVER,
	RETRY,
	GO_TITLE,
	SELECTBACK1,
	SELECTBACK2,
	EFFECT_SLASH,
	MAX
};

class TextureManager {
private:
	static std::unordered_map<TEXTURE, ID3D11ShaderResourceView*> m_LoadTexturePool;
	static std::unordered_map<TEXTURE, const wchar_t*> m_ReservTexturePool;

	static void LoadTexture(const TEXTURE& textureName, const wchar_t* fileName);
public:
	static void Init();
	static void Uninit();
	static void ReservTexture(const TEXTURE& texture, const wchar_t* fileName);
	static ID3D11ShaderResourceView*& GetTexture(const TEXTURE& texture);
};