#include "extrSwordBarst.h"
#include "staticMeshObject/equipmentObject.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
#include "character/player.h"

constexpr float LIFE_TIME = 0.3f;
constexpr float DEFALUT_SCALE = 1.2f;

void ExtrSwordBarst::CreateParticleEffect(const float& deltaTime)
{
	// 作成順的にUpdateで作成しないといけない
	if (m_PlayerSwordCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;
		Player* player = objManager->GetPlayer();

		m_PlayerSwordCache = player->GetSwordChache();
	}

	if (m_PlayerSwordCache == nullptr) return;

	XMMATRIX swordMatrix = m_PlayerSwordCache->GetRotationMatrixStaticMesh();

	XMFLOAT3 swordFoward = {};
	XMStoreFloat3(&swordFoward, swordMatrix.r[1]);

	int count = 0;

	m_ParticleLifeTime = LIFE_TIME;

	//パーティクルの発射
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (!m_Particle[i].enable)
		{
			m_Particle[i].enable = true;
			m_Particle[i].position = m_PlayerSwordCache->GetPos();
			m_Particle[i].velocity.x = (rand() % 100) * swordFoward.x;
			m_Particle[i].velocity.y = (rand() % 50 - 25) * swordFoward.y;
			m_Particle[i].velocity.z = (rand() % 100) * swordFoward.z;
			m_Particle[i].scale = { DEFALUT_SCALE,DEFALUT_SCALE,DEFALUT_SCALE };
			m_Particle[i].lifetime = m_ParticleLifeTime;
			m_Particle[i].color = { 0.2f,0.7f,0.98f,1.0f };

			count++;
			if (count > 20)
			{
				break;
			}
		}
	}
}

void ExtrSwordBarst::UpdateParticleEffect(const float& deltaTime)
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
