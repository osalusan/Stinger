#include "mawJLaygoBattleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode .h"
#include "behaviorNode/sequenceNode .h"
#include "behaviortaskNode/dashAtThePlayerTask.h"
#include "behaviorTaskNode/swipingTask.h"
#include "behaviortaskNode/checkRangeTask.h"
#include "behaviortaskNode/roaringTask.h"
#include "behaviorTaskNode/checkHealthTask.h"
#include "behaviorTaskNode/deadTask.h"
#include "behaviorTaskNode/jumpAttackTask.h"
#include "behaviorTaskNode/waitTask.h"

void MawJLaygoBattleTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	Player* player = objectManager->GetPlayer();
	if (player == nullptr) return;

	// -------------------------- �r�w�C�r�A�c���[�̍쐬 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	SequenceNode* healthNode = rootNode->AddNodeChild<SequenceNode>();
	if (healthNode == nullptr) return;

	// �̗͂̊m�F
	healthNode->AddTaskChild<CheckHealthTask>(boss, player);
	// ���S
	healthNode->AddTaskChild<DeadTask>(boss, player);

	SelectorNode* attackNode = rootNode->AddNodeChild<SelectorNode>();
	if (attackNode == nullptr) return;

	// �ҋ@
	attackNode->AddTaskChild<WaitTask>(boss, player);
	// �͈͂̊m�F
	attackNode->AddTaskChild<CheckRangeTask>(boss, player);
	// �U��
	attackNode->AddTaskChild<SwipingTask>(boss, player);
	attackNode->AddTaskChild<JumpAttackTask>(boss, player);
	attackNode->AddTaskChild<RoaringTask>(boss, player);
	// �U���s��
	attackNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// ��ԍŌ��
	CreateRoot(rootNode);
}
