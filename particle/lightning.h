#pragma once
#include "particleEmitter.h"

class Lightning final :public ParticleEmiter
{
private:
	virtual void CreateParticleEffect(const float& deltaTime)override;
	virtual void UpdateParticleEffect(const float& deltaTime)override;

	virtual void ReservTexture()override;
public:
	using ParticleEmiter::ParticleEmiter;
};