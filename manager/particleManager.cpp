#include "particleManager.h"
#include "particle/particleEmitter.h"

ParticleManager::~ParticleManager()
{
	for (int layer = 0; layer < static_cast<int>(PARTICLE::MAX); layer++)
	{
		for (ParticleEmiter* particle : m_ParticleObjects[layer])
		{
			delete particle;
			particle = nullptr;
		}
	}
	m_ParticleObjects->clear();
}

void ParticleManager::Init()
{
	for (int layer = 0; layer < static_cast<int>(PARTICLE::MAX); layer++)
	{
		for (ParticleEmiter* particle : m_ParticleObjects[layer])
		{
			if (particle == nullptr) continue;

			particle->Init();
		}
	}
}

void ParticleManager::Uninit()
{
	for (int layer = 0; layer < static_cast<int>(PARTICLE::MAX); layer++)
	{
		for (ParticleEmiter* particle : m_ParticleObjects[layer])
		{
			if (particle == nullptr) continue;

			particle->Uninit();
		}
	}
}

void ParticleManager::Update(const float& deltaTime)
{
	for (int layer = 0; layer < static_cast<int>(PARTICLE::MAX); layer++)
	{
		for (ParticleEmiter* particle : m_ParticleObjects[layer])
		{
			if (particle == nullptr) continue;

			particle->Update(deltaTime);
		}
	}
}

void ParticleManager::Draw()
{
	for (int layer = 0; layer < static_cast<int>(PARTICLE::MAX); layer++)
	{
		for (ParticleEmiter* particle : m_ParticleObjects[layer])
		{
			if (particle == nullptr) continue;

			particle->Draw();
		}
	}
}