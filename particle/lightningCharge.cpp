#include "lightningCharge.h"

void LightningCharge::CreateParticleEffect(const float& deltaTime)
{
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (!m_Particle[i].enable)
		{
			m_Particle[i].enable = true;
			m_Particle[i].position = m_Position;
			m_Particle[i].velocity.x = (rand() % 100 - 50) * 0.3f;
			m_Particle[i].velocity.y = (rand() % 100 - 50) * 0.3f;
			m_Particle[i].velocity.z = (rand() % 100 - 50) * 0.3f;
			m_Particle[i].scale = m_Scale;
			m_Particle[i].lifetime = 1.5f;
			break;
		}
	}
}

void LightningCharge::UpdateParticleEffect(const float& deltaTime)
{
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
