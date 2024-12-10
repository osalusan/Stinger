#pragma once
#include "main/main.h"
#define PARTICLE_MAX (500)

class Camera;
class ParticleEmiter
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };

	// 描画関連
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	Camera* m_CameraCache = nullptr;

	struct PARTICLE {
		bool enable;
		float lifetime;
		float age;
		XMFLOAT4 color;
		XMFLOAT3 position;
		XMFLOAT3 velocity;
		XMFLOAT3 scale;
		XMFLOAT3 maxScale;
		XMFLOAT3 upScale;
		XMFLOAT3 num;
	};

	PARTICLE m_Particle[PARTICLE_MAX] = {};

	bool m_Mix = false;//加算合成のオンオフ
	float m_Count = 0;
public:
	ParticleEmiter();
	~ParticleEmiter();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();
};