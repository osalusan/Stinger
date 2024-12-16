#include "mawJLaygoBattleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "manager\objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode .h"
#include "behaviortaskNodes/dashAtThePlayerTask.h"
#include "behaviortaskNodes/checkRangeTask.h"
#include "behaviortaskNodes/rollAttackTask.h"
#include "behaviortaskNodes/roaringTask.h"

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

	rootNode->AddTaskChild<CheckRangeTask>(boss, player);

	rootNode->AddTaskChild<RollAttackTask>(boss, player);

	rootNode->AddTaskChild<RoaringTask>(boss, player);

	rootNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// 一番最後に
	CreateRoot(rootNode);
}
