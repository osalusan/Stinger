#include "mawJLaygoBattleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "behaviorNode/sequenceNode.h"
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

	// -------------------------- ビヘイビアツリーの作成 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("体力管理シーケンス");
	if (healthSeqNode == nullptr) return;

	// 体力の確認
	healthSeqNode->AddTaskChild<CheckHealthTask>(boss, player);
	// 死亡
	healthSeqNode->AddTaskChild<DeadTask>(boss, player);

	BehaviorNode* attackSelNode = rootNode->AddNodeChild<SelectorNode>("攻撃管理セレクター");
	if (attackSelNode == nullptr) return;

	// 待機
	attackSelNode->AddTaskChild<WaitTask>(boss, player);
	// 範囲の確認
	attackSelNode->AddTaskChild<CheckRangeTask>(boss, player);
	// 攻撃
	attackSelNode->AddTaskChild<SwipingTask>(boss, player);
	attackSelNode->AddTaskChild<JumpAttackTask>(boss, player);
	attackSelNode->AddTaskChild<RoaringTask>(boss, player);
	// 攻撃不可
	attackSelNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// 一番最後に
	CreateRoot(rootNode);
}
