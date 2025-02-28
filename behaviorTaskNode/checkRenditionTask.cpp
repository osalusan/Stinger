#include "checkRenditionTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "camera/camera.h"
#include "scene/scene.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"

constexpr float LENGTH_RENDITION_START = 27.0f;
constexpr float POS_Y_RENDITION_START = -LENGTH_RENDITION_START * 0.4f;
constexpr float TARGET_POS_Y_RENDITION_START = LENGTH_RENDITION_START * 0.35f;

void CheckRenditionTask::Init()
{
	m_TaskName = "演出管理タスク";
	m_AcceptRendition = true;

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	m_CameraCache = objManager->GetCamera();
}

NODE_STATE CheckRenditionTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (!CheckRunningNode(node))
	{
		return NODE_STATE::FAILURE;
	}

	if (m_TaskCount >= m_Children.size())
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* child = m_Children[m_TaskCount];

	// ゲーム開始の演出
	if (m_TaskCount == 0 && m_AcceptRendition)
	{
		if (child != nullptr)
		{
			m_CurrentState = child->Update(deltaTime);
			child->SetCurrentState(m_CurrentState);
		}
	}

	if (m_CurrentState == NODE_STATE::RUNNING)
	{
		// カメラの設定
		if (m_CameraCache != nullptr)
		{
			m_CameraCache->SetRendition(true);
			m_CameraCache->SetRenditionLength(LENGTH_RENDITION_START);
			m_CameraCache->SetRenditionPosY(POS_Y_RENDITION_START);
			m_CameraCache->SetRenditionTargetPosY(TARGET_POS_Y_RENDITION_START);
		}
	}

	// 次のタスクを対象に
	if (m_CurrentState == NODE_STATE::SUCCESS)
	{
		m_TaskCount++;
		m_AcceptRendition = false;
		m_CameraCache->SetRendition(false);
	}

	return m_CurrentState;
}
