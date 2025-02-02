#include "mawJLaygoBattleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "behaviorNode/sequenceNode.h"
#include "behaviortaskNode/dashAtThePlayerTask.h"
#include "behaviorTaskNode/leftSwipingTask.h"
#include "behaviorTaskNode/rightSwipingTask.h"
#include "behaviortaskNode/checkRangeTask.h"
#include "behaviortaskNode/roaringTask.h"
#include "behaviorTaskNode/checkHealthTask.h"
#include "behaviorTaskNode/deadTask.h"
#include "behaviorTaskNode/jumpAttackTask.h"
#include "behaviorTaskNode/waitTask.h"
#include "behaviorTaskNode/checkAttackParry.h"
#include "behaviorTaskNode/parryRecoilTask.h"
#include "behaviorTaskNode/airLightningBallTask.h"
#include "behaviorTaskNode/rightPunchTask.h"
#include "behaviorTaskNode/shortRangeAttackTask.h"

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

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("�̗͊Ǘ��V�[�P���X");
	if (healthSeqNode == nullptr) return;

	// �̗͂̊m�F
	healthSeqNode->AddTaskChild<CheckHealthTask>(boss, player);
	// ���S
	healthSeqNode->AddTaskChild<DeadTask>(boss, player);

	BehaviorNode* parryRecoilNode = rootNode->AddNodeChild<SequenceNode>("�p���B�Ǘ��V�[�P���X");
	if (parryRecoilNode == nullptr) return;

	// �p���B�̊m�F
	BehaviorNode* ceckParry = parryRecoilNode->AddTaskChild<CheckAttackParryTask>(boss, player);
	if (ceckParry == nullptr) return;
	// �p���B���ꂽ
	ceckParry->AddTaskChild<ParryRecoilTask>(boss, player);

	BehaviorNode* attackSelNode = rootNode->AddNodeChild<SelectorNode>("�U���Ǘ��Z���N�^�[");
	if (attackSelNode == nullptr) return;

	// �ҋ@
	attackSelNode->AddTaskChild<WaitTask>(boss, player);
	// �͈͂̊m�F
	attackSelNode->AddTaskChild<CheckRangeTask>(boss, player);
	// �U��
	BehaviorNode* shortAttackTask = attackSelNode->AddTaskChild<ShortRangeAttackTask>(boss, player);
	shortAttackTask->AddTaskChild<RightPunchTask>(30,boss, player);
	BehaviorNode* leftSwiping = shortAttackTask->AddTaskChild<LeftSwipingTask>(70,boss, player);
	if (leftSwiping == nullptr) return;
	leftSwiping->AddTaskChild<RightSwipingTask>(boss, player);
	BehaviorNode* jumpAttack = attackSelNode->AddTaskChild<JumpAttackTask>(boss, player);
	jumpAttack->AddTaskChild<AirLightningBallTask>(boss, player);
	attackSelNode->AddTaskChild<RoaringTask>(boss, player);
	// �U���s��
	attackSelNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// ��ԍŌ��
	CreateRoot(rootNode);
}
