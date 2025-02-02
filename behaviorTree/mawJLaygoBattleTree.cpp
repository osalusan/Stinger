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

	// -------------------------- ビヘイビアツリーの作成 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("体力管理シーケンス");
	if (healthSeqNode == nullptr) return;

	// 体力の確認
	healthSeqNode->AddTaskChild<CheckHealthTask>(boss, player);
	// 死亡
	healthSeqNode->AddTaskChild<DeadTask>(boss, player);

	BehaviorNode* parryRecoilNode = rootNode->AddNodeChild<SequenceNode>("パリィ管理シーケンス");
	if (parryRecoilNode == nullptr) return;

	// パリィの確認
	BehaviorNode* ceckParry = parryRecoilNode->AddTaskChild<CheckAttackParryTask>(boss, player);
	if (ceckParry == nullptr) return;
	// パリィされた
	ceckParry->AddTaskChild<ParryRecoilTask>(boss, player);

	BehaviorNode* attackSelNode = rootNode->AddNodeChild<SelectorNode>("攻撃管理セレクター");
	if (attackSelNode == nullptr) return;

	// 待機
	attackSelNode->AddTaskChild<WaitTask>(boss, player);
	// 範囲の確認
	attackSelNode->AddTaskChild<CheckRangeTask>(boss, player);
	// 攻撃
	BehaviorNode* shortAttackTask = attackSelNode->AddTaskChild<ShortRangeAttackTask>(boss, player);
	shortAttackTask->AddTaskChild<RightPunchTask>(30,boss, player);
	BehaviorNode* leftSwiping = shortAttackTask->AddTaskChild<LeftSwipingTask>(70,boss, player);
	if (leftSwiping == nullptr) return;
	leftSwiping->AddTaskChild<RightSwipingTask>(boss, player);
	BehaviorNode* jumpAttack = attackSelNode->AddTaskChild<JumpAttackTask>(boss, player);
	jumpAttack->AddTaskChild<AirLightningBallTask>(boss, player);
	attackSelNode->AddTaskChild<RoaringTask>(boss, player);
	// 攻撃不可
	attackSelNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// 一番最後に
	CreateRoot(rootNode);
}
