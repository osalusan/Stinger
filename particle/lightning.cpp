#include "lightning.h"
#include "manager/textureManager.h"

void Lightning::CreateParticleEffect(const float& deltaTime)
{
	//パーティクルの発射
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		const float& randValue = (rand() % 100 - 50) * 0.003f;
		if (!m_Particle[i].enable)
		{
			m_Particle[i].enable = true;
			m_Particle[i].position.x = m_Position.x;
			m_Particle[i].position.y = m_Position.y;
			m_Particle[i].position.z = m_Position.z;
			m_Particle[i].scale = m_Scale;
			m_Particle[i].lifetime = 0.2f;
			break;
		}
	}
}

void Lightning::UpdateParticleEffect(const float& deltaTime)
{
	//パーティクルの処理
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].enable)
		{
			m_Particle[i].position.x += m_Particle[i].velocity.x * deltaTime;
			m_Particle[i].position.y += m_Particle[i].velocity.y * deltaTime;
			m_Particle[i].position.z += m_Particle[i].velocity.z * deltaTime;

			m_Particle[i].lifetime -= deltaTime;
			if (m_Particle[i].lifetime <= 0)
			{
				m_Particle[i].enable = false;
			}
		}
	}
}

void Lightning::ReservTexture()
{
	m_Texture = TEXTURE::PARTICLE_LIGHTNING;
	TextureManager::ReservTexture(m_Texture, L"asset\\texture\\lightning.png");
}
