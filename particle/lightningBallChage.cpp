#include "lightningBallChage.h"

constexpr float LENGTH_VALUE = 0.4f;
void LightningBallCharge::CreateParticleEffect(const float& deltaTime)
{
	if (!m_ChargeEnable) return;
	int count = 0;

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (!m_Particle[i].enable)
		{
			m_Particle[i].enable = true;
			const float& rangeX = static_cast<float>((rand() % 100) - 50);
			const float& rangeZ = static_cast<float>((rand() % 100) - 50);
			const float& rangeY = static_cast<float>((rand() % 100) - 50);
			m_Particle[i].position.x = m_Position.x + (rangeX * LENGTH_VALUE);
			m_Particle[i].position.y = m_Position.y + (rangeY * LENGTH_VALUE);
			m_Particle[i].position.z = m_Position.z + (rangeZ * LENGTH_VALUE);
			m_Particle[i].velocity.x = -(rangeX * LENGTH_VALUE) / m_LifeTime;
			m_Particle[i].velocity.y = -(rangeY * LENGTH_VALUE) / m_LifeTime;
			m_Particle[i].velocity.z = -(rangeZ * LENGTH_VALUE) / m_LifeTime;
			m_Particle[i].scale = m_Scale;
			m_Particle[i].lifetime = m_LifeTime;
			m_Particle[i].color = { 0.55f,0.4f,1.0f,1.0f };
			count++;
			if (count > 100)
			{
				m_ChargeEnable = false;
				break;
			}
		}
	}
}

void LightningBallCharge::UpdateParticleEffect(const float& deltaTime)
{
	m_CurrentTime += deltaTime;
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

void LightningBallCharge::Start(const float& time)
{
	m_ChargeEnable = true;
	m_LifeTime = time;
	m_Enable = true;
	m_CurrentTime = 0.0f;
}

void LightningBallCharge::End()
{
	ParticleEmiter::End();
	m_ChargeEnable = false;
	m_LifeTime = 0.0f;
}

bool LightningBallCharge::Finish()
{
	if (m_LifeTime == 0.0f) return false;

	if (m_CurrentTime > m_LifeTime)
	{
		End();
		return true;
	}
	return false;
}

