#include "spiralBlueorb.h"

constexpr float MAX_LIFETIME = 3.4f;
constexpr float DEFAULT_SPEED = 25.03f;
constexpr float RING_SIZE_VALUE = 0.15f;
constexpr XMFLOAT4 DEFAULT_COLOR = { 0.39f,0.75f,0.98f,1.0f };
constexpr float DEFAULT_SIZE = 0.2f;

// ---------------------------------- private ----------------------------------
void SpiralBlueOrb::CreateParticleEffect(const float& deltaTime)
{
	int f = 0;
	m_ParticleLifeTime = MAX_LIFETIME;
	for (int i = 0; i < PARTICLE_MAX; i++)
	{	
		if (!m_Particle[i].enable)
		{
			const float& ringSize = (rand() % 50) * RING_SIZE_VALUE;

			m_Particle[i].enable = true;
			m_Particle[i].position.x = m_Position.x + sinf(ringSize) + cosf(ringSize);
			m_Particle[i].position.y = m_Position.y + cosf(ringSize) - sinf(ringSize); ;
			m_Particle[i].position.z = m_Position.z + cosf(ringSize) - sinf(ringSize);
			m_Particle[i].velocity.x = 0.0f;
			m_Particle[i].velocity.y = 0.0f;
			m_Particle[i].velocity.z =- DEFAULT_SPEED;
			m_Particle[i].scale = { DEFAULT_SIZE ,DEFAULT_SIZE ,DEFAULT_SIZE };
			m_Particle[i].color = DEFAULT_COLOR;
			m_Particle[i].lifetime = m_ParticleLifeTime;

			f++;
			if (f >= 4)
			{
				f = 0;
				break;
			}
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
