#include "spiralBlueorb.h"

constexpr float MAX_LIFETIME = 2.4f;
constexpr float SPEED_VALUE = 0.03f;
constexpr float SIZE_VALUE = 0.03f;
constexpr XMFLOAT4 DEFAULT_COLOR = { 0.39f,0.75f,0.98f,1.0f };

// ---------------------------------- private ----------------------------------
void SpiralBlueOrb::CreateParticleEffect(const float& deltaTime)
{
	for (int i = 0; i < PARTICLE_MAX; i++)
	{	
		if (!m_Particle[i].enable)
		{
			const float& size = (rand() % 50) * SIZE_VALUE;
			m_Particle[i].enable = true;
			m_Particle[i].position = m_Position;
			m_Particle[i].velocity.x = (GetRandom() % 50) * SPEED_VALUE;
			m_Particle[i].velocity.y = (GetRandom() % 50) * SPEED_VALUE;
			m_Particle[i].velocity.z = (GetRandom() % 50) * SPEED_VALUE;
			m_Particle[i].scale = { size ,size ,size };
			m_Particle[i].color = DEFAULT_COLOR;
			m_Particle[i].lifetime = MAX_LIFETIME;
			break;
		}
	}
}

void SpiralBlueOrb::UpdateParticleEffect(const float& deltaTime)
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
