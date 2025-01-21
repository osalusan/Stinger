#include "waitTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
// TODO : �����O�ɍ폜�\�� / �f�o�b�O�p
#include "manager/inputManager.h"


void WaitTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\idle_MawJ.fbx", "idle_MawJ");
	m_TaskName = "�ҋ@";
}

NODE_STATE WaitTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		m_Wait = true;
		return NODE_STATE::FAILURE;
	}

	if (!m_Wait)
	{
		return NODE_STATE::FAILURE;
	}

	// TODO : �����O�ɍ폜�\�� / �f�o�b�O�p
	if (InputManager::GetKeyPress('V'))
	{
		m_PlayerCache->TakeDamage(1.0f);
	}

	if (m_CurrentTime < m_BossCache->GetMaxWaitTime())
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// ��Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// ��Ԃ��폜
			m_CurrentTime = 0.0f;
			m_Wait = false;
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}