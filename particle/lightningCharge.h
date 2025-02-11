#pragma once
#include "particleEmitter.h"

class LightningCharge final :public ParticleEmiter
{
private:
	virtual void CreateParticleEffect(const float& deltaTime)override;
	virtual void UpdateParticleEffect(const float& deltaTime)override;

	virtual void ReservTexture()override {};
public:
	using ParticleEmiter::ParticleEmiter;
};