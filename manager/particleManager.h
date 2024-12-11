#pragma once
#include <list>
#include <vector>

// ‹ó”’‚Ì”z—ñ‚ª¶‚Ü‚ê‚Ä‚µ‚Ü‚¤‚©‚çNONE‚Í’Ç‰Á‚µ‚È‚¢
enum class PARTICLE
{
	SPIRAL_BLUEORB = 0,
	MAX
};

// ‘O•ûéŒ¾
class ParticleEmiter;

class ParticleManager final
{
private:
	std::list<ParticleEmiter*> m_ParticleObjects[static_cast<int>(PARTICLE::MAX)] = {};
public:
	~ParticleManager();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void GetParticleObjects(std::vector<ParticleEmiter*>(&objectList)[static_cast<int>(PARTICLE::MAX)])
	{
		for (int i = 0; i < static_cast<int>(PARTICLE::MAX); i++)
		{
			objectList[i].insert(objectList[i].end(), m_ParticleObjects[i].begin(), m_ParticleObjects[i].end());
		}
	}

	// ˆø”–³‚µ‚Å’Ç‰Á
	template <typename T>
	void AddParticleObject(const PARTICLE& layer)
	{
		T* particleObject = new T;
		if (particleObject == nullptr) return;

		particleObject->Init();
		m_ParticleObjects[static_cast<int>(layer)].emplace_back(particleObject);
	}

	// ˆø”•t‚«‚Ìê‡‚Í‚±‚Á‚¿
	template <typename T, typename... Arg>
	void AddParticleObjectArg(const PARTICLE& layer, Arg&&...args)
	{
		T* particleObject = new T(std::forward<Arg>(args)...);
		if (particleObject == nullptr) return;

		particleObject->Init();
		m_ParticleObjects[static_cast<int>(layer)].emplace_back(particleObject);
	}
};