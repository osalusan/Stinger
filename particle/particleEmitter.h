#pragma once
#include "object/gameObject.h"
#define PARTICLE_MAX (800)

// 前方宣言
enum class TEXTURE;
class Camera;
// パーティクル用の基底クラス
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

	// パーティクル一つ一つのデータ
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

	bool m_Mix = false;							// 加算合成のオンオフ
	bool m_Enable = true;						// パーティクル全体の判定
	bool m_ReservEnable = false;				// 指定の秒数後に有効無効
	float m_ParticleLifeTime = 0.0f;			// パーティクルが生成できなくなる
	float m_CurrentLifeTime = 0.0f;				// 寿命の経過時間

	virtual void CreateParticleEffect(const float& deltaTime) = 0;		// 生成時の座標などを設定
	virtual void UpdateParticleEffect(const float& deltaTime) = 0;		// 移動などを設定

	virtual void ReservTexture() = 0;
public:
	ParticleEmiter();
	// パーティクルの座標の起点となる所を設定
	ParticleEmiter(const XMFLOAT3& pos);
	ParticleEmiter(const XMFLOAT3& pos,const bool& disable);

	ParticleEmiter(const bool& disable);
	~ParticleEmiter();
	void Init();
	void Update(const float& deltaTime);
	void Draw();

	virtual void End();
	virtual void Start();

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