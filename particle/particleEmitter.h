#pragma once
#include "object/gameObject.h"
#define PARTICLE_MAX (1000)

enum class TEXTURE;

class Camera;
class ParticleEmiter :public GameObject
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };

	// 描画関連
	ID3D11Buffer* m_VertexBuffer = NULL;

	TEXTURE m_Texture;						// 使用テクスチャ / コンストラクタで初期化

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
	bool m_Enable = true;

	virtual void CreateParticleEffect(const float& deltaTime) = 0;
	virtual void UpdateParticleEffect(const float& deltaTime) = 0;

	virtual void ReservTexture() = 0;
public:
	ParticleEmiter();
	ParticleEmiter(const XMFLOAT3& pos);
	ParticleEmiter(const XMFLOAT3& pos,const bool& disable);
	ParticleEmiter(const bool& disable);
	~ParticleEmiter();
	void Init();
	void Update(const float& deltaTime);
	void Draw();

	void SetEnable(const bool& flag)
	{
		m_Enable = flag;
	}
	const bool& GetEnable()const
	{
		return m_Enable;
	}

	void SetPos(const XMFLOAT3& pos)
	{
		m_Position = pos;
	}
};