#include "loadIdleTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/loadScene.h"
#include "camera/camera.h"

void LoadIdleTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\loadIdle_MawJ.fbx", "loadIdle_MawJ");
}

NODE_STATE LoadIdleTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	if (m_CameraCache == nullptr)
	{
		LoadScene* scene = SceneManager::GetLoadScene();
		if (scene == nullptr) return NODE_STATE::FAILURE;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return NODE_STATE::FAILURE;
		Camera* camera = objManager->GetCamera();
		if (camera == nullptr) return NODE_STATE::FAILURE;

		m_CameraCache = camera;
	}
	
	if (m_CameraCache != nullptr)
	{
		m_BossCache->RotToTarget(m_CameraCache,deltaTime);
	}

	m_BossCache->ChangeAnimation(m_AnimeName);

	return NODE_STATE::SUCCESS;
}