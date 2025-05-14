#pragma once
#include "particleEmitter.h"

class LightningBallCharge final :public ParticleEmiter
{
private:
	virtual void CreateParticleEffect(const float& deltaTime)override;
	virtual void UpdateParticleEffect(const float& deltaTime)override;

	virtual void ReservTexture()override {};

	float m_LifeTime = 0.0f;
	float m_CurrentTime = 0.0f;
	bool m_ChargeEnable = false;
public:
	using ParticleEmiter::ParticleEmiter;
	void Start(const float& time);
	void End();
	bool Finish();
};