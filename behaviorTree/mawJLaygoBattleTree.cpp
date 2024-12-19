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

void MawJLaygoBattleTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	GameScene* scene = SceneManager::GetScene<GameScene>();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	Player* player = objectManager->GetPlayer();
	if (player == nullptr) return;

	// -------------------------- ビヘイビアツリーの作成 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	SequenceNode* healthNode = rootNode->AddNodeChild<SequenceNode>();
	if (healthNode == nullptr) return;

	healthNode->AddTaskChild<CheckHealthTask>(boss, player);

	healthNode->AddTaskChild<DeadTask>(boss, player);

	SelectorNode* attackNode = rootNode->AddNodeChild<SelectorNode>();
	if (attackNode == nullptr) return;

	attackNode->AddTaskChild<CheckRangeTask>(boss, player);

	attackNode->AddTaskChild<SwipingTask>(boss, player);

	attackNode->AddTaskChild<JumpAttackTask>(boss, player);

	attackNode->AddTaskChild<RoaringTask>(boss, player);

	attackNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// 一番最後に
	CreateRoot(rootNode);
}
