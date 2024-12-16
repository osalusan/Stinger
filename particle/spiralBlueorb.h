#pragma once
#include "particleEmitter.h"

class SpiralBlueOrb final:public ParticleEmiter
{
private:
	virtual void CreateParticleEffect(const float& deltaTime)override;
	virtual void UpdateParticleEffect(const float& deltaTime)override;
public:
	using ParticleEmiter::ParticleEmiter;
};